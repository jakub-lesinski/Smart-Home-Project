package com.example.smarthome

import android.os.Bundle
import android.widget.Toast
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import com.example.smarthome.databinding.ActivityTemperatureBinding

class TemperatureActivity : AppCompatActivity(), BluetoothCommunicationManager.BluetoothDataListener {

    private lateinit var binding: ActivityTemperatureBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        binding = ActivityTemperatureBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Rejestracja listenera
        BluetoothCommunicationManager.registerListener(this)

        // Rozpoczęcie odbierania danych
        BluetoothCommunicationManager.startReceiving()

        binding.btnSet.setOnClickListener {
            val wpisanyTekst = binding.ET.text.toString()
            binding.ExpTempText2.text = "$wpisanyTekst°C"
            val command = "TM$wpisanyTekst" // Tworzymy komendę do wysłania
            BluetoothCommunicationManager.sendData(command)
        }
    }

    override fun onDataReceived(data: String) {
        val firstChar = data[0]
        val secondChar = data[1]
        val thirdChar = data[2]
        val fourthChar = data[3]

        if(firstChar == 'T' && secondChar == 'M') {
            val temp = "${thirdChar}${fourthChar}"
            binding.CurrTempText2.text = "$temp°C"
        }

    }

    override fun onError(error: String) {
        Toast.makeText(this, error, Toast.LENGTH_SHORT).show()
    }

    override fun onDestroy() {
        super.onDestroy()
        BluetoothCommunicationManager.unregisterListener(this)
    }
}