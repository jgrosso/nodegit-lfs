type successResponse 'a = {
  payload: 'a,
  raw: string
};

type errorResponse = {
  errno: int,
  raw: string,
  stderr: string
};

type response 'a =
| Success (successResponse 'a)
| Error errorResponse