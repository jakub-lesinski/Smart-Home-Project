package com.example.smarthome

import android.os.Bundle
import android.widget.Toast
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import com.example.smarthome.databinding.ActivityKitchenBinding
import com.example.smarthome.databinding.ActivityLivingRoomBinding
import com.example.smarthome.BluetoothConnectionManager
import java.io.IOException

class KitchenActivity : AppCompatActivity() {

    private lateinit var binding: ActivityKitchenBinding
    private var btSocket = BluetoothConnectionManager.btSocket

    override fun onCreate(savedInstanceState: Bundle?) {
        binding = ActivityKitchenBinding.inflate(layoutInflater)
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(binding.root)

        var language = intent.getIntExtra("KEY",0)

        if(language==0)
        {
            binding.buttonLight.text="Light"
        }
        if(language==1)
        {
            binding.buttonLight.text="Lampka"
        }

        binding.buttonLight.setOnClickListener {
              sendCommand("c")
        }

    }

    private fun sendCommand(input: String) {
        btSocket?.let {
            try {
                it.outputStream.write(input.toByteArray())
                Toast.makeText(this, "Message sent: $input", Toast.LENGTH_SHORT).show()
            } catch (e: IOException) {
                e.printStackTrace()
                Toast.makeText(this, "Can't send message", Toast.LENGTH_SHORT).show()
            }
        } ?: run {
            Toast.makeText(this, "Bluetooth socket is not connected", Toast.LENGTH_SHORT).show()
        }
    }
}