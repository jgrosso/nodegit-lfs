type t = Node.Buffer.t;
external concat: array t => t = "Buffer.concat" [@@bs.val];
external length: t => int = "" [@@bs.send];
external toString: t => string = "" [@@bs.send];