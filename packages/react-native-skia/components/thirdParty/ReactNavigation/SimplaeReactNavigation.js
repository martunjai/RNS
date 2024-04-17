import { AppRegistry} from 'react-native';
import React, { useEffect, useState } from 'react';
import { View, Text, TouchableOpacity, StyleSheet } from 'react-native';
import { NavigationContainer, DefaultTheme } from '@react-navigation/native';
import { createNativeStackNavigator } from '@react-navigation/native-stack';
import { createStackNavigator } from '@react-navigation/stack';
import { enableScreens } from 'react-native-screens';

// Disable native screens
enableScreens(false);

// Custom header component
const CustomHeader = ({ navigation, route }) => {
  const isHomeScreen = route.name === 'Home';

  return (
    <View style={styles.header}>
      {isHomeScreen ? (
        <View style={styles.homeButton} />
      ) : (
        <TouchableOpacity onPress={() => navigation.goBack()} style={styles.backButton}>
          <View style={styles.arrow} />
        </TouchableOpacity>
      )}
      <View style={styles.headerTitleContainer}>
        <Text style={styles.headerTitle}>{route.name}</Text>
      </View>
      {!isHomeScreen && (
        <TouchableOpacity onPress={() => navigation.navigate('Home')} style={styles.homeButton}>
          <Text style={styles.homeButtonText}>Home</Text>
        </TouchableOpacity>
      )}
    </View>
  );
};

// Create a native stack navigator
const createStackNavigatorWithHeader = () => {

  useNativeStack = true
  useCustomHeader = true

  const Stack = useNativeStack ? createNativeStackNavigator() : createStackNavigator();
  const screenOptions = useCustomHeader
    ? {
        header: CustomHeader,
      }
    : {};

  return (
    <Stack.Navigator screenOptions={screenOptions}>
      <Stack.Screen name="Home" component={HomeScreen} />
      <Stack.Screen name="Details" component={DetailsScreen} />
      <Stack.Screen name="Settings" component={SettingsScreen} />
    </Stack.Navigator>
  );
};

// Screen components
const HomeScreen = ({ navigation }) => (
  <View style={styles.container}>
    <View style={styles.contentContainer}>
      <Text style={styles.title}>Home Screen</Text>
      <TouchableOpacity onPress={() => navigation.navigate('Details')} style={styles.button}>
        <Text style={styles.buttonText}>Go to Details</Text>
      </TouchableOpacity>
    </View>
  </View>
);

const DetailsScreen = ({ navigation }) => (
  <View style={styles.container}>
    <View style={styles.contentContainer}>
      <Text style={styles.title}>Details Screen</Text>
      <TouchableOpacity onPress={() => navigation.navigate('Settings')} style={styles.button}>
        <Text style={styles.buttonText}>Go to Settings</Text>
      </TouchableOpacity>
    </View>
  </View>
);

const SettingsScreen = ({ navigation }) => (
  <View style={styles.container}>
    <View style={styles.contentContainer}>
      <Text style={styles.title}>Settings Screen</Text>
      <TouchableOpacity onPress={() => navigation.goBack()} style={styles.button}>
        <Text style={styles.buttonText}>Go Back</Text>
      </TouchableOpacity>
    </View>
  </View>
);

const App = () => (
  <NavigationContainer theme={DefaultTheme}>
    {createStackNavigatorWithHeader()}
  </NavigationContainer>
);

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#f0f0f0',
  },
  contentContainer: {
    backgroundColor: '#fff',
    borderRadius: 10,
    padding: 20,
    borderWidth: 1,
    borderColor: '#ddd',
    shadowColor: '#000',
    shadowOffset: {
      width: 0,
      height: 2,
    },
    shadowOpacity: 0.25,
    shadowRadius: 3.84,
    elevation: 5,
  },
  title: {
    fontSize: 24,
    fontWeight: 'bold',
    marginBottom: 20,
  },
  header: {
    flexDirection: 'row',
    alignItems: 'center',
    justifyContent: 'center',
    height: 100,
    backgroundColor: '#007bff',
    paddingTop: 50,
  },
  backButton: {
    position: 'absolute',
    top: 50,
    left: 10,
    width: 30,
    height: 30,
    alignItems: 'center',
    justifyContent: 'center',
    borderRadius: 15,
    backgroundColor: '#fff',
  },
  arrow: {
    width: 10,
    height: 10,
    borderLeftWidth: 2,
    borderBottomWidth: 2,
    transform: [{ rotate: '45deg' }],
    borderColor: '#333',
  },
  headerTitleContainer: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
  },
  headerTitle: {
    fontSize: 18,
    fontWeight: 'bold',
    color: '#fff',
  },
  homeButton: {
    position: 'absolute',
    top: 50,
    right: 10,
    paddingHorizontal: 10,
    paddingVertical: 5,
  },
  homeButtonText: {
    fontSize: 16,
    fontWeight: 'bold',
    color: '#fff',
  },
  button: {
    padding: 10,
    backgroundColor: '#007bff',
    borderRadius: 5,
    marginTop: 10,
  },
  buttonText: {
    color: '#fff',
    fontSize: 16,
  },
});

export default App;

AppRegistry.registerComponent('SimpleViewApp', () => App);
