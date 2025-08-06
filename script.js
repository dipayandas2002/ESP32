import { initializeApp } from "https://www.gstatic.com/firebasejs/12.0.0/firebase-app.js";
import { getAnalytics } from "https://www.gstatic.com/firebasejs/12.0.0/firebase-analytics.js";
import { getDatabase, ref, set, get, child, update, onValue } from "https://www.gstatic.com/firebasejs/12.0.0/firebase-database.js";

const firebaseConfig = {
  apiKey: "AIzaSyAsStgGUdvG2ol_PThdSvq_gwqj9PNGtfI",
  authDomain: "esp32ddas.firebaseapp.com",
  databaseURL: "https://esp32ddas-default-rtdb.firebaseio.com",
  projectId: "esp32ddas",
  storageBucket: "esp32ddas.firebasestorage.app",
  messagingSenderId: "46440929820",
  appId: "1:46440929820:web:6d0791c65748f1b20d1dd9",
  measurementId: "G-WKKEEKCGN0"
};

const app = initializeApp(firebaseConfig);
getAnalytics(app);
const db = getDatabase(app);

const lightOne = document.getElementById('lightOne');
const lightTwo = document.getElementById('lightTwo');
const labelOne = document.getElementById('labelOne');
const labelTwo = document.getElementById('labelTwo');

function updateBackground() {
  if (lightOne.checked && lightTwo.checked) document.body.style.backgroundColor = '#fff7cc';
  else if (lightOne.checked || lightTwo.checked) document.body.style.backgroundColor = '#62ade2';
  else document.body.style.backgroundColor = '#111827';
}

function updateTextColors() {
  labelOne.className = lightOne.checked ? 'text-black' : 'text-white';
  labelTwo.className = lightTwo.checked ? 'text-black' : 'text-white';
}

window.updateAll = function() {
  updateBackground();
  updateTextColors();
};

lightOne.addEventListener('change', () => {
  update(ref(db, 'Light'), { One: lightOne.checked });
});

lightTwo.addEventListener('change', () => {
  update(ref(db, 'Light'), { Two: lightTwo.checked });
});

onValue(ref(db, 'Light/One'), (snapshot) => {
  lightOne.checked = snapshot.val() === true;
  updateAll();
});

onValue(ref(db, 'Light/Two'), (snapshot) => {
  lightTwo.checked = snapshot.val() === true;
  updateAll();
});
