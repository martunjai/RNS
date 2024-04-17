import { AppRegistry} from 'react-native';

import React, { useRef, useEffect, useState } from "react";
import { TouchableOpacity, TouchableHighlight, View, Text, StyleSheet, Pressable, findNodeHandle, TouchableWithoutFeedback} from "react-native";

const App = () => {
  const sourceTouchableOpacity1Ref = useRef();
  const destinationTouchableOpacity1Ref = useRef();
  const sourceTouchableHighlightRef = useRef();
  const destinationTouchableOpacity2Ref = useRef();
  const sourcePressableRef = useRef();
  const destinationTouchableOpacity3Ref = useRef();
  const sourceTouchableWithoutHighlightRef = useRef();
  const destinationTouchableOpacity4Ref = useRef();
  const sourceTouchableOpacity1Node = useRef(null);
  const destinationTouchableOpacity1Node = useRef(null);
  const sourceTouchableHighlightNode = useRef(null);
  const destinationTouchableOpacity2Node = useRef(null);
  const sourcePressableNode = useRef(null);
  const destinationTouchableOpacity3Node = useRef(null);
  const sourceTouchableWithoutHighlightNode = useRef(null);
  const destinationTouchableOpacity4Node = useRef(null);
  const [isTouchableHighlightFocused, setTouchableHighlightFocused] = useState(false);
  const [isPressableFocused, setPressableFocused] = useState(false);

  useEffect(() => {
    sourceTouchableOpacity1Node.current = findNodeHandle(sourceTouchableOpacity1Ref.current);
    destinationTouchableOpacity1Node.current = findNodeHandle(destinationTouchableOpacity1Ref.current);
    sourceTouchableHighlightNode.current = findNodeHandle(sourceTouchableHighlightRef.current);
    destinationTouchableOpacity2Node.current = findNodeHandle(destinationTouchableOpacity2Ref.current);
    sourcePressableNode.current = findNodeHandle(sourcePressableRef.current);
    destinationTouchableOpacity3Node.current = findNodeHandle(destinationTouchableOpacity3Ref.current);
    sourceTouchableWithoutHighlightNode.current = findNodeHandle(sourceTouchableWithoutHighlightRef.current);
    destinationTouchableOpacity4Node.current = findNodeHandle(destinationTouchableOpacity4Ref.current);

    if (
      sourceTouchableOpacity1Node.current &&
      destinationTouchableOpacity1Node.current &&
      sourceTouchableHighlightNode.current &&
      destinationTouchableOpacity2Node.current &&
      sourcePressableNode.current &&
      destinationTouchableOpacity3Node.current &&
      sourceTouchableWithoutHighlightNode.current &&
      destinationTouchableOpacity4Node.current
    ) {
      sourceTouchableOpacity1Ref.current.setNativeProps({
        nextFocusUp: destinationTouchableOpacity1Node.current,
        nextFocusDown: destinationTouchableOpacity1Node.current,
        nextFocusLeft: destinationTouchableOpacity1Node.current,
        nextFocusRight: destinationTouchableOpacity1Node.current,
      });

      sourceTouchableHighlightRef.current.setNativeProps({
        nextFocusUp: destinationTouchableOpacity2Node.current,
        nextFocusDown: destinationTouchableOpacity2Node.current,
        nextFocusLeft: destinationTouchableOpacity2Node.current,
        nextFocusRight: destinationTouchableOpacity2Node.current,
      });

      sourcePressableRef.current.setNativeProps({
        nextFocusUp: destinationTouchableOpacity3Node.current,
        nextFocusDown: destinationTouchableOpacity3Node.current,
        nextFocusLeft: destinationTouchableOpacity3Node.current,
        nextFocusRight: destinationTouchableOpacity3Node.current,
      });
       // TouhableWithoutFeedback doesnt support
      //sourceTouchableWithoutHighlightRef.current.setNativeProps({
      //  nextFocusUp: destinationTouchableOpacity4Node.current,
      //  nextFocusDown: destinationTouchableOpacity4Node.current,
      //  nextFocusLeft: destinationTouchableOpacity4Node.current,
      //  nextFocusRight: destinationTouchableOpacity4Node.current,
      //});
    }
  }, []);

  const handleTouchableHighlightFocus = () => {
    setTouchableHighlightFocused(true);
  };

  const handleTouchableHighlightBlur = () => {
    setTouchableHighlightFocused(false);
  };

  const handlePressableFocus = () => {
    setPressableFocused(true);
  };

  const handlePressableBlur = () => {
    setPressableFocused(false);
  };

  return (
    <View style={styles.container}>
      <Text style={styles.heading}>NextFocus Property Test</Text>

      <View style={styles.rowContainer}>
        <TouchableOpacity
          style={styles.button}
          ref={sourceTouchableOpacity1Ref}
        >
          <Text style={styles.buttonText}>Touchable Opacity</Text>
        </TouchableOpacity>
        <TouchableOpacity
          style={styles.button}
          ref={destinationTouchableOpacity1Ref}
        >
          <Text style={styles.buttonText}>Touchable Opacity Destination</Text>
        </TouchableOpacity>
      </View>
      <View style={styles.rowContainer}>
        <TouchableHighlight
          style={[styles.button, isTouchableHighlightFocused && styles.highlightedButton]}
          ref={sourceTouchableHighlightRef}
          onFocus={handleTouchableHighlightFocus}
          onBlur={handleTouchableHighlightBlur}

        >
          <Text style={styles.buttonText}>Touchable Highlight</Text>
        </TouchableHighlight>
        <TouchableOpacity
          style={styles.button}
          ref={destinationTouchableOpacity2Ref}
        >
          <Text style={styles.buttonText}>Touchable Highlight Destination</Text>
        </TouchableOpacity>
      </View>
      <View style={styles.rowContainer}>
        <Pressable
          style={[styles.button, isPressableFocused && styles.highlightedButton]}
          ref={sourcePressableRef}
          onFocus={handlePressableFocus}
          onBlur={handlePressableBlur}
        >
          <Text style={styles.buttonText}>Pressable </Text>
        </Pressable>
        <TouchableOpacity
          style={styles.button}
          ref={destinationTouchableOpacity3Ref}
        >
          <Text style={styles.buttonText}>Pressable Destination</Text>
        </TouchableOpacity>
      </View>
      <View style={styles.rowContainer}>
        <TouchableWithoutFeedback
          style={styles.button}
          ref={sourceTouchableWithoutHighlightRef}
        >
          <Text style={styles.buttonText}>Touchable Without Feedback</Text>
        </TouchableWithoutFeedback>
        <TouchableOpacity
          style={styles.button}
          ref={destinationTouchableOpacity4Ref}
        >
          <Text style={styles.buttonText}>Touchable Without Feedback doesnt support</Text>
        </TouchableOpacity>
      </View>
    </View>
  );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: "center",
    alignItems: "center",
  },
  rowContainer: {
    flexDirection: "row",
    marginBottom: 20,
  },
  heading: {
    fontSize: 20,
    fontWeight: "bold",
    marginBottom: 20,
  },
  button: {
    marginHorizontal: 10,
    padding: 10,
    backgroundColor: "lightblue",
    borderRadius: 5,
  },
  buttonText: {
    fontSize: 16,
    fontWeight: "bold",
  },
  highlightedButton: {
    backgroundColor: "orange",
  },
});

export default App;


AppRegistry.registerComponent('SimpleViewApp', () => App);
