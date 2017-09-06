let badCoreResponse: string = "-1";
let badRegexParseResult: string = "-2";
let badVersion: string = "0";

let lfsAttribute: string = "filter=lfs diff=lfs merge=lfs";

let lfsFilterName: string = "nodegit_lfs";

module MinimumVersions = {
  let git: string = "1.8.5";
  let lfs: string = "2.0.0";
};

module Regex = {
  let git: Js.Re.t = [%bs.re {|/(?:git\s+version\s+)(\d+)\.(\d+)\.(\d+)/|}];
  let lfs: Js.Re.t = [%bs.re {|/(?:git-lfs\/\s*)?(\d+)(?:.(\d+))?(?:.(\d+))?.*/|}];
  let passphrase: Js.Re.t = [%bs.re {|/passphrase/g|}];
  let password: Js.Re.t = [%bs.re {|/password/g|}];
  let permissionDenied: Js.Re.t = [%bs.re {|/permission\s+denied\s+\(.+\)\./g|}];
  let skippedBytes: Js.Re.t = [%bs.re {|/[\d]+\s+B\s+(?=skipped)/g|}];
  let skippedFiles: Js.Re.t = [%bs.re {|/[\d]\s+(?=skipped)/g|}];
  let totalBytes: Js.Re.t = [%bs.re {|/[\d]+\s+B/g|}];
  let totalFiles: Js.Re.t = [%bs.re {|/[\d]\s+(?=files)/g|}];
  let track: Js.Re.t = [%bs.re {|/([a-zA-Z*.]+(?="))/g|}];
  let username: Js.Re.t = [%bs.re {|/username/g|}];
};