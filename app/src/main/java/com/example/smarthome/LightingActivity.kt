package com.example.smarthome

import android.os.Bundle
import android.widget.SeekBar
import android.widget.Toast
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import com.example.smarthome.databinding.ActivityLightingBinding

class LightingActivity : AppCompatActivity() {
    private lateinit var binding: ActivityLightingBinding
    private var currentProgress1: Int = 0
    private var currentProgress2: Int = 0
    private var currentProgress3: Int = 0

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityLightingBinding.inflate(layoutInflater)
        enableEdgeToEdge()
        setContentView(binding.root)

        BluetoothCommunicationManager.registerListener(object : BluetoothCommunicationManager.BluetoothDataListener {
            override fun onDataReceived(data: String) {}

            override fun onError(error: String) {
                Toast.makeText(this@LightingActivity, error, Toast.LENGTH_SHORT).show() // Pokazuje błąd, jeśli wystąpił problem z Bluetooth
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
                if (currentProgress1<10)
                {
                    val command = "X00$currentProgress1" // Tworzymy komendę do wysłania
                    BluetoothCommunicationManager.sendData(command) // Wysyłamy komendę przez BluetoothCommunicationManager
                }
                if(currentProgress1>9 && currentProgress1<100)
                {
                    val command = "X0$currentProgress1" // Tworzymy komendę do wysłania
                    BluetoothCommunicationManager.sendData(command) // Wysyłamy komendę przez BluetoothCommunicationManager
                }
                if (currentProgress1==100)
                {
                    val command = "X$currentProgress1" // Tworzymy komendę do wysłania
                    BluetoothCommunicationManager.sendData(command) // Wysyłamy komendę przez BluetoothCommunicationManager
                }
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
                currentProgress2 = progress
            }

            override fun onStartTrackingTouch(seekBar: SeekBar?) {}

            override fun onStopTrackingTouch(seekBar: SeekBar?) {
                if (currentProgress2<10)
                {
                    val command = "Y00$currentProgress2"
                    BluetoothCommunicationManager.sendData(command)
                }
                if(currentProgress2>9 && currentProgress2<100)
                {
                    val command = "Y0$currentProgress2"
                    BluetoothCommunicationManager.sendData(command)
                }
                if (currentProgress2==100)
                {
                    val command = "Y$currentProgress2"
                    BluetoothCommunicationManager.sendData(command)
                }
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
                if (currentProgress3<10)
                {
                    val command = "Z00$currentProgress3" // Tworzymy komendę do wysłania
                    BluetoothCommunicationManager.sendData(command) // Wysyłamy komendę przez BluetoothCommunicationManager
                }
                if(currentProgress3>9 && currentProgress3<100)
                {
                    val command = "Z0$currentProgress3" // Tworzymy komendę do wysłania
                    BluetoothCommunicationManager.sendData(command) // Wysyłamy komendę przez BluetoothCommunicationManager
                }
                if (currentProgress3==100)
                {
                    val command = "Z$currentProgress3" // Tworzymy komendę do wysłania
                    BluetoothCommunicationManager.sendData(command) // Wysyłamy komendę przez BluetoothCommunicationManager
                }
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