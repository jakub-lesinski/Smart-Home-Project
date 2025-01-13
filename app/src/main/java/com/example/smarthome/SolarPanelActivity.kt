package com.example.smarthome

import android.os.Bundle
import android.widget.Toast
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import com.example.smarthome.databinding.ActivitySolarpanelBinding

class SolarPanelActivity : AppCompatActivity(), BluetoothCommunicationManager.BluetoothDataListener {

    private lateinit var binding: ActivitySolarpanelBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivitySolarpanelBinding.inflate(layoutInflater)
        enableEdgeToEdge()
        setContentView(binding.root)

        binding.switchPower.setOnCheckedChangeListener { _, isChecked ->
            val command = if (isChecked) "PW01" else "PW00"
            BluetoothCommunicationManager.sendData(command)
        }

        BluetoothCommunicationManager.registerListener(this)
        BluetoothCommunicationManager.startReceiving()
    }

    override fun onDataReceived(data: String) {
        runOnUiThread {
            binding.PowerText2.text = "$data W"
        }
    }

    override fun onError(error: String) {
        runOnUiThread {
            Toast.makeText(this, "Błąd: $error", Toast.LENGTH_SHORT).show()
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        BluetoothCommunicationManager.unregisterListener(this)
    }
}