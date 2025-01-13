package com.example.smarthome

import android.os.Bundle
import android.widget.SeekBar
import android.widget.Toast
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import com.example.smarthome.databinding.ActivityLightningBinding

class LightningActivity : AppCompatActivity() {
    private lateinit var binding: ActivityLightningBinding
    private var currentProgress1: Int = 0
    private var currentProgress2: Int = 0
    private var currentProgress3: Int = 0

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityLightningBinding.inflate(layoutInflater)
        enableEdgeToEdge()
        setContentView(binding.root)

        BluetoothCommunicationManager.registerListener(object : BluetoothCommunicationManager.BluetoothDataListener {
            override fun onDataReceived(data: String) {}

            override fun onError(error: String) {
                Toast.makeText(this@LightningActivity, error, Toast.LENGTH_SHORT).show() // Pokazuje błąd, jeśli wystąpił problem z Bluetooth
            }
        })

        // Obsługa przełącznika w kuchnii
        binding.switchK.setOnCheckedChangeListener { _, isChecked ->
            val command = if (isChecked) "LK01" else "LK00" // Jeśli switch jest włączony, wysyłamy "LK01", jeśli wyłączony, "LK00"
            BluetoothCommunicationManager.sendData(command) // Wysyłamy komendę przez BluetoothCommunicationManager
        }

        // Obsługa jasności w kuchnii
        binding.seekBarK.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                currentProgress1 = progress // Zapisujemy postęp z SeekBar
            }

            override fun onStartTrackingTouch(seekBar: SeekBar?) {}

            override fun onStopTrackingTouch(seekBar: SeekBar?) {
                Toast.makeText(this@LightningActivity, "Final Progress: $currentProgress1", Toast.LENGTH_SHORT).show()
                val command = "1$currentProgress1" // Tworzymy komendę do wysłania
                BluetoothCommunicationManager.sendData(command) // Wysyłamy komendę przez BluetoothCommunicationManager
            }
        })

        // Obsługa przełącznika w salonie
        binding.switchL.setOnCheckedChangeListener { _, isChecked ->
            val command = if (isChecked) "LL01" else "LL00"
            BluetoothCommunicationManager.sendData(command)
        }

        // Obsługa jasności w salonie
        binding.seekBarL.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                currentProgress2 = progress // Zapisujemy postęp z SeekBar
            }

            override fun onStartTrackingTouch(seekBar: SeekBar?) {}

            override fun onStopTrackingTouch(seekBar: SeekBar?) {
                Toast.makeText(this@LightningActivity, "Final Progress: $currentProgress2", Toast.LENGTH_SHORT).show()
                val command = "2$currentProgress2"
                BluetoothCommunicationManager.sendData(command)
            }
        })

        // Obsługa przełącznika w garażu
        binding.switchG.setOnCheckedChangeListener { _, isChecked ->
            val command = if (isChecked) "LG01" else "LG00"
            BluetoothCommunicationManager.sendData(command)
        }

        // Obsługa jasności w garażu
        binding.seekBarG.setOnSeekBarChangeListener(object : SeekBar.OnSeekBarChangeListener {
            override fun onProgressChanged(seekBar: SeekBar?, progress: Int, fromUser: Boolean) {
                currentProgress3 = progress // Zapisujemy postęp z SeekBar
            }
            override fun onStartTrackingTouch(seekBar: SeekBar?) {}

            override fun onStopTrackingTouch(seekBar: SeekBar?) {
                Toast.makeText(this@LightningActivity, "Final Progress: $currentProgress3", Toast.LENGTH_SHORT).show()
                val command = "3$currentProgress3"
                BluetoothCommunicationManager.sendData(command)
            }
        })
    }

    // Metoda wywoływana przy zniszczeniu Activity
    override fun onDestroy() {
        super.onDestroy()
        // Wyrejestrowanie listenera po zniszczeniu Activity
        BluetoothCommunicationManager.unregisterListener(object : BluetoothCommunicationManager.BluetoothDataListener {
            override fun onDataReceived(data: String) {}
            override fun onError(error: String) {}
        })
    }
}