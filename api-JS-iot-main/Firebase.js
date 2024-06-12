import { initializeApp } from 'firebase/app';

const firebaseConfig = {
  apiKey: "AIzaSyDGbnhVQGbMgCp9t47uLzJnnLAeYRfgg70",
  authDomain: "wallwatcher-319e4.firebaseapp.com",
  databaseURL: "https://wallwatcher-319e4-default-rtdb.europe-west1.firebasedatabase.app",
  projectId: "wallwatcher-319e4",
  storageBucket: "wallwatcher-319e4.appspot.com",
  messagingSenderId: "27046952871",
  appId: "1:27046952871:web:d0bcc90e0f56da52068082",
  measurementId: "G-455Z6V85L7"
};

const firebase = initializeApp(firebaseConfig);


export default firebase;