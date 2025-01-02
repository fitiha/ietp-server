import express from "express";
import bodyParser from "body-parser";
import dotenv from "dotenv";
import cors from "cors";
import { WebSocketServer } from "ws";

const app = express();
app.use(cors());
dotenv.config();
const PORT = process.env.PORT || 3000;

app.use(bodyParser.json());

const server = app.listen(PORT, "0.0.0.0", () => {
  console.log(`Server is running on http://localhost:${PORT}`);
});

const wss = new WebSocketServer({ server });

wss.on("connection", (ws) => {
  console.log("New WebSocket connection");
});

app.post("/data", (req, res) => {
  const data = req.body;
  console.log("Received data:", data);
  res.status(200).send("Data received successfully");

  // Broadcast data to all connected WebSocket clients
  wss.clients.forEach((client) => {
    if (client.readyState === client.OPEN) {
      client.send(JSON.stringify(data));
    }
  });
});
