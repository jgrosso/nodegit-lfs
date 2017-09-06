type promptType =
| Passphrase
| Password
| Username;

let buildSocketPath () =>
  Tmp.dir ()
    |> Js.Promise.then_ (fun tmpPath => {
      let prefix = Process.platform === "win32" ? "\\\\.\\pipe" : "";
      let cleanedPath = Process.platform === "win32" ? Js.String.replaceByRe [%bs.re {|/C:\\|\.tmp/g|}] tmpPath "" : tmpPath;
      Js.Promise.resolve ((Node.Path.join [| prefix, cleanedPath, "echo.sock" |]):Js.String.t);
    });

type credentialsCallback = (string => string => bool => unit);
type promptCallback = string => credentialsCallback => unit;
let buildCredentialsCallbackProcess
  (spawnedProcess:Node.Child_process.option)
  (callback:promptCallback)
  (reject:(Error.error => unit))
  (chunk:Node.Buffer.t)
  :string => {
  let credentialsCallback (promptType:promptType) (username:string) (password:string) (cancel:bool) :unit =>
      if cancel {
        ChildProcess.destroy spawnedProcess;
        reject Error.Cancel;
      } else {
        ChildProcess.write
          spawnedProcess
          (
            switch promptType {
            | Username => username
            | _ => password
            }
          );
      };
  
  let output = chunk
    |> Buffer.toString
    |> Js.String.trim
    |> Js.String.toLowerCase;

  let promptType = if (Js.Re.test output Constants.Regex.username) {
    Username
  } else if (Js.Re.test output Constants.Regex.password) {
    Password
  } else {
    Passphrase
  };
  callback output (credentialsCallback promptType);

  output;
};

let buildSocket
  (size:int)
  (closeProcess:(unit => unit))
  (socketName:string)
  (mainResolve:(Node.Buffer.t => unit))
  (mainReject:Error.error => unit)
  :(Js.Promise.t string)
  => Promise.create (fun resolve reject => {
  let bufferList = [||];
  let currentSize = ref 0;
  let errorInfo: ref (option (Error.exitCode, Error.error => unit)) = ref None;

  let handleErrorWith
    (rejectionHandler:(Error.error => unit))
    (exitCode:Error.exitCode)
    :unit => {
    errorInfo := Some (exitCode, rejectionHandler);
  };

  let closed (_code:int) :unit => {
    closeProcess ();

    switch !errorInfo {
    | Some (exitCode, rejectionHandler) => rejectionHandler (Error.ExitCode exitCode)
    | None => mainResolve (Buffer.concat bufferList)
    }
  };

  let socket: ref (option Net.Socket.t) = ref None;
  let socketServer: Net.SocketServer.t =
    Net.SocketServer.create (fun newSocket => {
      socket := Some newSocket;
    })
    |> Net.SocketServer.on (`listening (fun () => resolve socketName))
    |> Net.SocketServer.on (`error (handleErrorWith reject))
    |> Net.SocketServer.on (`close closed);
  switch !socket {
  | Some socket =>
    socket
      |> Net.Socket.on (`close closed)
      |> Net.Socket.on (`data (fun data => {
        currentSize := !currentSize + Buffer.length data;
        Js.Array.push data bufferList |> ignore;
        if (!currentSize === size) {
          Net.SocketServer.close socketServer (fun (_code:int) => ());
        }
      }))
      |> Net.Socket.on (`error (handleErrorWith mainReject))
      |> ignore
  | _ => failwith "`socket` should have been assigned!";
  }
});

let spawnShell
  (command:string)
  (opts:Js.t {..})
  (size:int)
  (callback:option promptCallback)
  :(Js.Promise.t Node.Buffer.t) => {
  Promise.create (fun resolve reject => {
    buildSocketPath ()
      |> Js.Promise.then_ (fun socket => {
        let options = Ramda.mergeDeepRight opts { "encoding": Js.Null.from_opt None }
          |> Ramda.mergeDeepRight { "env": Process.env };
        let spawnedProcess: Node.Child_process.option = Pty.spawn DefaultShell.defaultShell [||] options;
        Js.Promise.all2 (
          buildSocket size (fun () => Pty.destroy spawnedProcess) socket resolve reject,
          Js.Promise.resolve spawnedProcess
        );
      })
      |> Js.Promise.then_ (fun (socketName, spawnedProcess) => {
        let processChunk: Buffer.t => string = switch (callback) {
        | Some promptCallback => buildCredentialsCallbackProcess spawnedProcess promptCallback reject
        | None => Buffer.toString
        };
        let commandSuffix: string = " " ^ (Process.platform === "win32" ? ">" : "| nc -U ") ^ socketName;
        Pty.write spawnedProcess (command ^ commandSuffix);
        Pty.write spawnedProcess Os.eol;
        spawnedProcess
          |> Pty.on (`data (fun chunk => processChunk chunk |> ignore))
          |> Pty.on (`error (fun exitCode => reject (Error.ExitCode exitCode)))
          |> ignore;
        Js.Promise.resolve ();
      })
      |> Promise.catch reject
      |> ignore;
  });
};