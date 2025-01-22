package com.example.smarthome

import android.content.Intent
import android.os.Bundle
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import com.example.smarthome.databinding.ActivityFunctionsBinding

class FunctionsActivity : AppCompatActivity() {

    private lateinit var binding: ActivityFunctionsBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        binding = ActivityFunctionsBinding.inflate(layoutInflater)
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(binding.root)

        binding.btnLightning.setOnClickListener {
            val explicitIntent = Intent(applicationContext, LightingActivity::class.java)
            startActivity(explicitIntent)
        }

        binding.btnTemperature.setOnClickListener {
            val explicitIntent = Intent(applicationContext, TemperatureActivity::class.java)
            startActivity(explicitIntent)
        }

        binding.btnShutters.setOnClickListener {
            val explicitIntent = Intent(applicationContext, ShuttersActivity::class.java)
            startActivity(explicitIntent)
        }

        binding.btnAlarm.setOnClickListener {
            val explicitIntent = Intent(applicationContext, AlarmActivity::class.java)
            startActivity(explicitIntent)
        }
    }
}

