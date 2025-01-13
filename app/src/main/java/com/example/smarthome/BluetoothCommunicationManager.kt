package com.example.smarthome

import android.bluetooth.BluetoothSocket
import android.util.Log
import kotlinx.coroutines.*
import java.io.IOException


object BluetoothCommunicationManager {
    var btSocket: BluetoothSocket? = null

    private val _listeners = mutableListOf<BluetoothDataListener>()
    private val coroutineScope = CoroutineScope(Dispatchers.IO)

    interface BluetoothDataListener {
        fun onDataReceived(data: String)
        fun onError(error: String)
    }

    // Rejestracja listenera
    fun registerListener(listener: BluetoothDataListener) {
        _listeners.add(listener)
    }

    // Usunięcie listenera
    fun unregisterListener(listener: BluetoothDataListener) {
        _listeners.remove(listener)
    }

    // Rozpoczęcie odbierania danych
    fun startReceiving() {
        btSocket?.let { socket ->
            coroutineScope.launch {
                val buffer = ByteArray(1024)
                val stringBuilder = StringBuilder()
                try {
                    val inputStream = socket.inputStream
                    while (true) {
                        val bytesRead = inputStream.read(buffer)
                        if (bytesRead > 0) {
                            val chunk = String(buffer, 0, bytesRead)
                            stringBuilder.append(chunk)

                            var newlineIndex: Int
                            while (stringBuilder.indexOf("\n").also { newlineIndex = it } != -1) {
                                val message = stringBuilder.substring(0, newlineIndex)
                                stringBuilder.delete(0, newlineIndex + 1)

                                // Powiadomienie wszystkich listenerów
                                withContext(Dispatchers.Main) {
                                    _listeners.forEach { it.onDataReceived(message) }
                                }
                            }
                        }
                    }
                } catch (e: IOException) {
                    Log.e("BluetoothManager", "Error receiving data", e)
                    withContext(Dispatchers.Main) {
                        _listeners.forEach { it.onError("Can't receive message: ${e.message}") }
                    }
                }
            }
        } ?: Log.e("BluetoothManager", "Bluetooth socket is not connected")
    }

    // Funkcja do wysyłania danych
    fun sendData(data: String) {
        btSocket?.let {
            try {
                it.outputStream.write(data.toByteArray())
                Log.d("BluetoothManager", "Message sent: $data")
            } catch (e: IOException) {
                Log.e("BluetoothManager", "Error sending data", e)
            }
        }
    }

    // Możliwość zatrzymania odbioru
    fun stopReceiving() {
        coroutineScope.cancel()
    }
}