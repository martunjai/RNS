import React, { useState, useEffect } from 'react';
import { AppRegistry, View, Text, StyleSheet } from 'react-native';
import { Appearance } from 'react-native';

const App = () => {
  const [colorScheme, setColorScheme] = useState(Appearance.getColorScheme());

  useEffect(() => {
    const handleAppearanceChange = ({ colorScheme }) => {
      setColorScheme(colorScheme);
    };

    Appearance.addChangeListener(handleAppearanceChange);

    return () => {
      Appearance.removeChangeListener(handleAppearanceChange);
    };
  }, []);

  const getBackgroundColor = () => {
    return colorScheme === 'dark' ? '#333' : '#f0f0f0';
  };

  const textColor = colorScheme === 'dark' ? '#fff' : '#000';

  return (
    <View style={[styles.container, { backgroundColor: getBackgroundColor() }]}>
      <Text style={[styles.text, { color: textColor }]}>Current Color Scheme: {colorScheme}</Text>
    </View>
  );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
  },
  text: {
    fontSize: 36,
    marginBottom: 20,
  },
});

export default App;

AppRegistry.registerComponent('SimpleViewApp', () => App);
