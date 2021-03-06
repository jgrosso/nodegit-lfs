import NodeGit from 'nodegit';
import path from 'path';
import { default as LFS } from '../../../build/src';
import prune from '../../../build/src/commands/prune';

describe('Prune', () => {
  it('does generate prune response', () => {
    const workdirPath = path.join(__dirname, '../../repos/workdir');
    const NodeGitLFS = LFS(NodeGit);

    return NodeGitLFS.Repository.open(workdirPath)
      .then(repo => prune(repo))
      .then(response => console.log(response));
  });
});
