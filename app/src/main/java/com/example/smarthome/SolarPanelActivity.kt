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
            val firstChar = data[0]
            val secondChar = data[1]
            val thirdChar = data[2]
            val fourthChar = data[3]
            val fifthChar = data[4]

            if(data.length == 4 && (firstChar == 'B') && (secondChar == 'T')) {
                if(fourthChar=='0') {
                    binding.BatteryImage.setImageResource(R.drawable.off)
                }

                if(fourthChar=='1')
                {
                    binding.BatteryImage.setImageResource(R.drawable.on)
                }
            }

            if(data.length==4 && firstChar == 'P' && secondChar == 'S') {
                if(fourthChar=='0') {
                    binding.PowerSupplyImage.setImageResource(R.drawable.off)
                }

                if(fourthChar=='1')
                {
                    binding.PowerSupplyImage.setImageResource(R.drawable.on)
                }
            }

            if(data.length==4 && firstChar == 'S' && secondChar == 'P') {
                if(fourthChar=='0') {
                    binding.SPImage.setImageResource(R.drawable.off)
                }

                if(fourthChar=='1')
                {
                    binding.SPImage.setImageResource(R.drawable.on)
                }
            }

            if(data.length==5 && firstChar == 'W') {
                val power = "${secondChar}${thirdChar}${fourthChar}${fifthChar}"
                binding.PowerText2.text = "$power W"
            }
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