import { AppRegistry} from 'react-native';
import React, { useRef, useEffect, useState } from 'react';
import { View, Text, Button, TouchableOpacity, StyleSheet, findNodeHandle } from 'react-native';

const App = () => {
  const sourceButtonRef = useRef(null);
  const destinationTouchableRef = useRef(null);
  const [buttonFocused, setButtonFocused] = useState(false);
  const [destinationTouchableNode, setDestinationTouchableNode] = useState(null);

  useEffect(() => {
    setDestinationTouchableNode(findNodeHandle(destinationTouchableRef.current));
  }, []);

  const handleSourceButtonFocus = () => {
    setButtonFocused(true);
  };

  const handleSourceButtonBlur = () => {
    setButtonFocused(false);
  };

  const handleSourceButtonPress = () => {
    if (destinationTouchableNode) {
      destinationTouchableRef.current.measureLayout(sourceButtonRef.current, (x, y) => {
        console.log('Destination Touchable component layout:', { x, y });
      });
    }
  };

  return (
    <View style={styles.container}>
      <Button
        title="Source Button"
        onPress={handleSourceButtonPress}
        onFocus={handleSourceButtonFocus}
        onBlur={handleSourceButtonBlur}
        ref={sourceButtonRef}
        style={[styles.button, buttonFocused && styles.buttonFocused]}
        nextFocusUp={destinationTouchableNode}
        nextFocusDown={destinationTouchableNode}
        nextFocusLeft={destinationTouchableNode}
        nextFocusRight={destinationTouchableNode}
      />
      <TouchableOpacity
        style={styles.destinationTouchable}
        ref={destinationTouchableRef}
      >
        <Text style={styles.destinationTouchableText}>Destination Touchable</Text>
      </TouchableOpacity>
    </View>
  );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
  },
  button: {
    marginVertical: 10,
    paddingHorizontal: 20,
    paddingVertical: 10,
    backgroundColor: 'lightblue',
    borderRadius: 5,
  },
  buttonFocused: {
    backgroundColor: 'blue',
  },
  destinationTouchable: {
    padding: 10,
    backgroundColor: 'lightgreen',
    borderRadius: 5,
    marginTop: 20,
  },
  destinationTouchableText: {
    fontSize: 16,
    fontWeight: 'bold',
  },
});

export default App;


AppRegistry.registerComponent('SimpleViewApp', () => App);
