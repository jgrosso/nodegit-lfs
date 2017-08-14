const { spawn } = require('child_process');

let serverPid = null;

module.exports = {
  start() {
    if (serverPid) {
      throw new Error('LFS test server has already been started!');
    }

    return new Promise((resolve, reject) => {
      const server = spawn('./start.sh', {
        cwd: __dirname,
        shell: true
      });
      server.stdout.on('data', (data) => {
        // Store outputted server PID
        const pid = data.toString().match(/pid=(\d+)/);
        if (pid) {
          serverPid = parseInt(pid[1], 10);
          return resolve();
        }

        // Handle Go errors
        const err = data.toString().match(/ err=(.*)/);
        if (err) {
          return reject(err[1]);
        }
      });
      server.stderr.on('data', (err) => {
        return reject(err.toString());
      });
    });
  },

  stop() {
    if (!serverPid) {
      throw new Error("LFS test server hasn't been started!");
    }

    process.kill(serverPid, 'SIGKILL');
  }
};
