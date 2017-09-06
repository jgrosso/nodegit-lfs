import {
  expect
} from 'chai';

import {
  Error
} from '../../../lib/src/constants';
import initialize from '../../../lib/src/callbacks/initialize';

describe('initialize', () => {
  describe('the default export', () => {
    it('returns `OK`', () => {
      expect(initialize()).to.equal(Error.CODE.OK);
    });
  });
});
