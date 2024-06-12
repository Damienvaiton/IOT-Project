import express from "express";
import cors from "cors";
import firebase from "./Firebase.js";
import { getDatabase, ref, set, get, onValue } from "firebase/database";

const app = express();
const db = getDatabase(firebase);
const distanceRef = ref(db, "/000000001/distance/distance");
const buzzerRef = ref(db, "/000000001/buzzer");

app.use(cors());
app.use(express.json());

app.post("/register-esp", function (req, res) {
	let id = req.query.id;
	set(ref(db, "/" + id), {
		esp: id,
	});
	res.send("Registered");
});

app.post("/set-distance", function (req, res) {
	console.log(req.body);
	let id = req.body.id;
	set(ref(db, id + "/distance"), {
		distance: req.body.distance,
		unit: "Meters",
	});
	res.send("Updated");
});

app.get("/distance", function (req, res) {
	get(distanceRef)
		.then((snapshot) => {
			if (snapshot.exists()) {
				const distanceData = snapshot.val();
				const distance = distanceData;
				res.status(200).send(String(distance));
			} else {
				res.status(404).send("No distance data available");
			}
		})
		.catch((error) => {
			console.error("Error retrieving distance data:", error);
			res.status(500).send("Error retrieving distance data");
		});
});

app.post("/set-tone-frequency", function (req, res) {
	let id = req.body.id;
	console.log(req.body.tone, req.body.frequency);
	set(ref(db, "/000000001/buzzer"), {
		tone: req.body.tone,
		frequency: req.body.frequency,
	});
	res.send("Updated");
});


app.get("/note-frequency", function (req, res) {
	get(buzzerRef)
		.then((snapshot) => {
			if (snapshot.exists()) {
				res.send(snapshot.val());
			} else {
				res.send("No data available");
			}
		})
		.catch((error) => {
			console.error(error);
		});
});

app.get("/buzzer-note", function (req, res) {
	get(buzzerRef)
		.then((snapshot) => {
			if (snapshot.exists() && snapshot.val().tone) {
				res.send(snapshot.val().tone);
			} else {
				res.status(404).send("Aucune donnée de note disponible");
			}
		})
		.catch((error) => {
			console.error(error);
			res
				.status(500)
				.send("Erreur lors de la récupération de la note du buzzer");
		});
});

// Route pour récupérer la fréquence du buzzer
app.get("/buzzer-frequency", function (req, res) {
	get(buzzerRef)
		.then((snapshot) => {
			if (snapshot.exists() && snapshot.val().frequency) {
				res.send(snapshot.val().frequency.toString());
			} else {
				res.status(404).send("Aucune donnée de fréquence disponible");
			}
		})
		.catch((error) => {
			console.error(error);
			res
				.status(500)
				.send("Erreur lors de la récupération de la fréquence du buzzer");
		});
});

app.listen(3000, () => console.log(`Server is live`));
