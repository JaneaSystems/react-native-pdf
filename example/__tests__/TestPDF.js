import { driver, By2 } from 'selenium-appium'
import { until } from 'selenium-webdriver';

const setup = require('../jest-setups/jest.setup');
jest.setTimeout(50000);

beforeAll(() => {
  return driver.startWithCapabilities(setup.capabilites);
});

afterAll(() => {
  return driver.quit();
});

describe('Test App', () => {

  test('PDF present', async () => {

  });

})
