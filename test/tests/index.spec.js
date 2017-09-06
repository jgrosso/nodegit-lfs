import {
  expect
} from 'chai';
import NodeGit from 'nodegit';

import checkout from '../../lib/src/commands/checkout';
import clone from '../../lib/src/commands/clone';
import fetch from '../../lib/src/commands/fetch';
import initialize from '../../lib/src/initialize';
import {
  core
} from '../../lib/src/commands/lfsCommands';
import list from '../../lib/src/commands/ls';
import prune from '../../lib/src/commands/prune';
import pull from '../../lib/src/commands/pull';
import push from '../../lib/src/commands/push';
import testPointer from '../../lib/src/commands/pointer';
import register from '../../lib/src/register';
import track from '../../lib/src/commands/track';
import unregister from '../../lib/src/unregister';
import untrack from '../../lib/src/commands/untrack';
import version from '../../lib/src/commands/version';
import {
  loadGitattributeFiltersFromRepo,
  repoHasLfs
} from '../../lib/src/helpers';
import {
  dependencyCheck
} from '../../lib/src/utils/checkDependencies';

// NOTE These tests depend on `LFS(NodeGit)` being called in the global `beforeEach`.
describe('index', () => {
  describe('the default export', () => {
    it('adds LFS to NodeGit permanently', () => {
      expect(NodeGit.LFS).to.eql({
        checkout,
        clone,
        core,
        dependencyCheck,
        fetch,
        filters: loadGitattributeFiltersFromRepo,
        initialize,
        list,
        NodeGit,
        register,
        repoHasLfs,
        testPointer,
        track,
        prune,
        pull,
        push,
        version,
        unregister,
        untrack
      });
    });
  });
});
