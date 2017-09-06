type exitCode = int;

module Socket = {
  type t;
  external on:
    ([ `close (exitCode => unit)
     | `data (Node.Buffer.t => unit)
     | `error (exitCode => unit)
     ]
     [@bs.string]
    ) =>
    t
    = "" [@@bs.send.pipe: t];
};

module SocketServer = {
  type t;
  external close: t => (int => unit) => unit = "" [@@bs.send];
  external create: (Socket.t => unit) => t = "createServer" [@@bs.module "net"];
  external listen: t => string => unit = "" [@@bs.send];
  external on:
    ([ `close (exitCode => unit)
     | `error (exitCode => unit)
     | `listening (unit => unit)
     ]
     [@bs.string]
    ) =>
    t
    = "" [@@bs.send.pipe: t];
};