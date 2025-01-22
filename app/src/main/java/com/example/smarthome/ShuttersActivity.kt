package com.example.smarthome

import android.os.Bundle
import android.widget.Toast
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import com.example.smarthome.databinding.ActivityShuttersBinding

class ShuttersActivity : AppCompatActivity() {
    private lateinit var binding: ActivityShuttersBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityShuttersBinding.inflate(layoutInflater)
        setContentView(binding.root)
        enableEdgeToEdge()

        // Rejestracja listenera Bluetooth
        BluetoothCommunicationManager.registerListener(object : BluetoothCommunicationManager.BluetoothDataListener {
            override fun onDataReceived(data: String) {}

            override fun onError(error: String) {
                Toast.makeText(this@ShuttersActivity,error, Toast.LENGTH_SHORT).show()
            }
        })

        // Obsługa przełącznika w kuchnii
        binding.switchK.setOnCheckedChangeListener { _, isChecked ->
            val command = if (isChecked) "SK01" else "SK00"
            BluetoothCommunicationManager.sendData(command)
        }

        // Obsługa przełącznika w salonie
        binding.switchL.setOnCheckedChangeListener { _, isChecked ->
            val command = if (isChecked) "SL01" else "SL00"
            BluetoothCommunicationManager.sendData(command)
        }

        // Obsługa przełącznika w garażu
        binding.switchG.setOnCheckedChangeListener { _, isChecked ->
            val command = if (isChecked) "SG01" else "SG00"
            BluetoothCommunicationManager.sendData(command)
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        // Wyrejestrowanie listenera po zniszczeniu Activity
        BluetoothCommunicationManager.unregisterListener(object : BluetoothCommunicationManager.BluetoothDataListener {
            override fun onDataReceived(data: String) {}
            override fun onError(error: String) {}
        })
    }
}