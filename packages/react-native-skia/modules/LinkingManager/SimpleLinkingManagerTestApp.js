import React, { useEffect, useState } from 'react';
import { AppRegistry, View, Text, Button, Linking, TextInput, StyleSheet } from 'react-native';

const App = () => {
  const [response, setResponse] = useState('');
  const [eventNotification, setEventNotification] = useState('');
  const [error, setError] = useState(null);
  const [urlValue, setUrlValue] = useState('https://www.example.com');

  useEffect(() => {
    // Adding event listener for 'url' event
    Linking.addEventListener('url', handleDeepLink);

    // Cleanup the event listener when the component unmounts
    return () => {
      Linking.removeEventListener('url', handleDeepLink);
    };
  }, []);

  const handleDeepLink = (event) => {
    // Handling deep link URLs
    setEventNotification('[url event] Received deep link: ' + event.url);
  };

  const setAppStatus = (responseValue, errorValue, notificationValue) => {
    setResponse(responseValue);
    setError(errorValue);
    setEventNotification(notificationValue);
  };

  const openURL = () => {
    const url = urlValue.trim();

    Linking.openURL(url)
      .then(() => {
        setAppStatus('URL opened successfully', null, null);
      })
      .catch((error) => {
        setAppStatus('', error, null);
      });
  };

  const openURLSync = async () => {
    const url = urlValue.trim();

    try {
      await Linking.openURL(url);
      setAppStatus('URL opened successfully', null, null);
    } catch (error) {
      setAppStatus('', error, null);
    }
  };

  const checkCanOpenURL = () => {
    const url = urlValue.trim();

    Linking.canOpenURL(url)
      .then((supported) => {
        if (supported) {
          setAppStatus('URL can be opened', null, null);
        } else {
          setAppStatus('URL cannot be opened', null, null);
        }
      })
      .catch((error) => {
        setAppStatus('', error, null);
      });
  };

  const checkCanOpenURLSync = async () => {
    const url = urlValue.trim();

    try {
      const supported = await Linking.canOpenURL(url);
      if (supported) {
        setAppStatus('URL can be opened', null, null);
      } else {
        setAppStatus('URL cannot be opened', null, null);
      }
    } catch (error) {
      setAppStatus('', error, null);
    }
  };

  const getInitialURL = () => {
    Linking.getInitialURL()
      .then((url) => {
        setAppStatus(`Initial URL: ${url}`, null, null);
      })
      .catch((error) => {
        setAppStatus('', error, null);
      });
  };

  const getInitialURLSync = async () => {
    try {
      const url = await Linking.getInitialURL();
      setAppStatus(`Initial URL: ${url}`, null, null);
    } catch (error) {
      setAppStatus('', error, null);
    }
  };

  const openSettings = () => {
    Linking.openSettings()
      .then(() => {
        setAppStatus('Opened settings', null, null);
      })
      .catch((error) => {
        setAppStatus('', error, null);
      });
  };

  const sendIntent = () => {
    Linking.sendIntent("")
      .then(() => {
        setAppStatus('Intent Sent', null, null);
      })
      .catch((error) => {
        setAppStatus('', error, null);
      });
  };

  const openSettingsSync = async () => {
    try {
      await Linking.openSettings();
      setAppStatus('Opened settings', null, null);
    } catch (error) {
      setAppStatus('', error, null);
    }
  };

  const renderError = () => {
    if (error) {
      return <Text style={styles.error}>{`Error: ${error.message}`}</Text>;
    }

    return null;
  };

  return (
    <View style={styles.container}>
      <Text style={styles.heading}>React Native Linking App</Text>
      <Text style={styles.label}>URL:</Text>
      <TextInput
        style={styles.input}
        value={urlValue}
        onChangeText={setUrlValue}
      />
      <Button title="Open URL" onPress={openURL} />
      <Button title="Check Can Open URL" onPress={checkCanOpenURL} />
      <Button title="Get Initial URL" onPress={getInitialURL} />
      <Button title="Open Settings" onPress={openSettings} />
      <Button title="Send Intent (Android Only)" onPress={sendIntent} />
      <Button title="Open URL (await)" onPress={openURLSync} />
      <Button title="Check Can Open URL (await)" onPress={checkCanOpenURLSync} />
      <Button title="Get Initial URL (await)" onPress={getInitialURLSync} />
      <Button title="Open Settings (await)" onPress={openSettingsSync} />
      <Text style={styles.response}>{response}</Text>
      <Text style={styles.response}>{eventNotification}</Text>
      {renderError()}
    </View>
  );
};

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    padding: 20,
  },
  heading: {
    fontSize: 20,
    fontWeight: 'bold',
    marginBottom: 20,
  },
  label: {
    fontSize: 16,
    fontWeight: 'bold',
    marginBottom: 5,
  },
  input: {
    width: '80%',
    height: 40,
    borderColor: 'gray',
    borderWidth: 1,
    marginBottom: 10,
    paddingHorizontal: 10,
  },
  response: {
    marginTop: 20,
    fontSize: 16,
    color: 'green',
    textAlign: 'center',
  },
  error: {
    marginTop: 10,
    fontSize: 16,
    color: 'red',
    textAlign: 'center',
  },
});

export default App;

AppRegistry.registerComponent('SimpleViewApp', () => App);
