import path from 'path';
import fs from 'fs';
import NodeGit from 'nodegit';
import { expect } from 'chai';
import { default as LFS } from '../../build/src';

const local = path.join.bind(path, __dirname);

describe('Initialize', () => {
  it('initialize is a promise', () => {
    const NodeGitLFS = LFS(NodeGit);
    const workdirPath = local('../repos/workdir');

    return NodeGitLFS.Repository.open(workdirPath)
      .then((repo) => {
        const init = NodeGitLFS.LFS.initialize(repo);
        expect(init).to.be.a('promise');
      });
  });

  it('creates .gitattributes for empty repo', () => {
    const NodeGitLFS = LFS(NodeGit);
    const emptydirPath = local('../repos/empty');
    //eslint-disable-next-line
    expect(fs.existsSync(path.join(emptydirPath, '.gitattributes'))).to.be.false;

    return NodeGitLFS.Repository.open(emptydirPath)
      .then(repo => NodeGitLFS.LFS.initialize(repo))
      .then(() => {
        // eslint-disable-next-line
        expect(fs.existsSync(path.join(emptydirPath, '.gitattributes'))).to.be.true;
      });
  });
});
