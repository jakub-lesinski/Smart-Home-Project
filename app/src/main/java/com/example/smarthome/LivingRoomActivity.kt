package com.example.smarthome

import android.os.Bundle
import android.widget.Toast
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import com.example.smarthome.databinding.ActivityLivingRoomBinding
import java.io.IOException
import com.example.smarthome.BluetoothConnectionManager

class LivingRoomActivity : AppCompatActivity() {

    private lateinit var binding: ActivityLivingRoomBinding
    private var btSocket = BluetoothConnectionManager.btSocket

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityLivingRoomBinding.inflate(layoutInflater)
        setContentView(binding.root)
        enableEdgeToEdge()

        var language = intent.getIntExtra("KEY",0)

        if(language==0)
        {
            binding.buttonLight2.text="Light"
        }
        if(language==1)
        {
            binding.buttonLight2.text="Lampka"
        }

        binding.buttonLight2.setOnClickListener {
            sendCommand("b")
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