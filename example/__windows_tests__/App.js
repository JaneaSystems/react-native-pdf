/**
 * @format
 */

import 'react-native';
import React from 'react';
import PDFExample from '../PDFExample';

// Note: test renderer must be required after react-native.
import renderer from 'react-test-renderer';

it('renders correctly', () => {
  renderer.create(<PDFExample />);
});