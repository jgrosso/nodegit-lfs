external on:
  ([ `data (Node.Buffer.t => unit)
   | `error (Error.exitCode => unit)
   ]
   [@bs.string]
  ) =>
  Node.Child_process.option
  = "" [@@bs.send.pipe: Node.Child_process.option];
external destroy: Node.Child_process.option => unit = "" [@@bs.send];
external spawn: DefaultShell.shellLocation => array string => Js.t {..} => Node.Child_process.option = "" [@@bs.module "node-pty"];
external write: Node.Child_process.option => string => unit = "" [@@bs.send];