import { AppRegistry} from 'react-native';

if (typeof queueMicrotask === 'undefined') {
    if (typeof setImmediate !== 'undefined') {
    // Fallback using setImmediate
    window.queueMicrotask = (callback) => {
    setImmediate(callback);
    };
    } else {
    // Fallback using setTimeout
    window.queueMicrotask = (callback) => {
    setTimeout(callback, 0);
    };
    }
  }

import React, { useState } from 'react';
import { View, Text, TouchableOpacity, StyleSheet } from 'react-native';
import { GestureHandlerRootView, TapGestureHandler, LongPressGestureHandler } from 'react-native-gesture-handler';

const App = () => {
  const [tapEvent, setTapEvent] = useState(null);
  const [stateChangeEvent, setStateChangeEvent] = useState(null);

  const handleTap = event => {
    setTapEvent(event.nativeEvent);
    console.log('Gesture event:', event.nativeEvent);
  };

  const handleStateChangeTap = event => {
    setStateChangeEvent(event.nativeEvent);
    console.log('Gesture State Change event:', event.nativeEvent);
  };

  return (
    <GestureHandlerRootView style={styles.container}>
      <Text style={styles.title}>Gesture Handler App</Text>
      <TapGestureHandler onHandlerStateChange={handleStateChangeTap} onGestureEvent={handleTap}>
        <TouchableOpacity style={styles.button}>
          <Text style={styles.buttonText}>Tap Me</Text>
        </TouchableOpacity>
      </TapGestureHandler>
      {tapEvent && (
        <View style={styles.eventContainer}>
          <Text style={styles.eventTitle}>Tap Event:</Text>
          <Text style={styles.eventText}>{JSON.stringify(tapEvent, null, 2)}</Text>
        </View>
      )}
      {stateChangeEvent && (
        <View style={styles.eventContainer}>
          <Text style={styles.eventTitle}>State Change Event:</Text>
          <Text style={styles.eventText}>{JSON.stringify(stateChangeEvent, null, 2)}</Text>
        </View>
      )}
    </GestureHandlerRootView>
  );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
  },
  title: {
    fontSize: 26,
    fontWeight: 'bold',
    marginBottom: 20,
  },
  button: {
    backgroundColor: 'blue',
    padding: 10,
    marginVertical: 10,
    borderRadius: 8,
  },
  buttonText: {
    color: 'white',
    fontSize: 16,
    fontWeight: 'bold',
  },
  eventContainer: {
    marginTop: 20,
    paddingHorizontal: 20,
  },
  eventTitle: {
    fontSize: 22,
    fontWeight: 'bold',
    marginBottom: 10,
  },
  eventText: {
    fontSize: 20,
    fontFamily: 'Courier',
    backgroundColor: '#f0f0f0',
    padding: 10,
    borderRadius: 8,
  },
});

export default App;



AppRegistry.registerComponent('SimpleViewApp', () => App);
