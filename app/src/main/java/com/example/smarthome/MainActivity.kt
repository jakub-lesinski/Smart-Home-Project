package com.example.smarthome

import android.Manifest
import android.annotation.SuppressLint
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothManager
import android.bluetooth.BluetoothSocket
import android.content.Context
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Bundle
import android.widget.Toast
import androidx.activity.result.ActivityResultCallback
import androidx.activity.result.ActivityResultLauncher
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import com.example.smarthome.databinding.ActivityMainBinding
import java.io.IOException
import java.util.*
import kotlin.concurrent.thread
import com.example.smarthome.BluetoothConnectionManager

class MainActivity : AppCompatActivity() {

    private lateinit var myBltDevice: BluetoothDevice
    private lateinit var binding: ActivityMainBinding
    private lateinit var bluetoothManager: BluetoothManager
    private lateinit var bluetoothAdapter: BluetoothAdapter
    private lateinit var takePermission: ActivityResultLauncher<String>
    private lateinit var takeResultLauncher: ActivityResultLauncher<Intent>
    private val MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB")
    private var btSocket: BluetoothSocket? = null
    var language =0


    @SuppressLint("SuspiciousIndentation")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        bluetoothManager = getSystemService(BLUETOOTH_SERVICE) as BluetoothManager
        bluetoothAdapter = bluetoothManager.adapter

        takePermission = registerForActivityResult(ActivityResultContracts.RequestPermission()) {
            if (it) {
                val intent = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
                takeResultLauncher.launch(intent)
            } else {
                Toast.makeText(applicationContext, "Bluetooth Permission is not Granted", Toast.LENGTH_SHORT).show()
            }
        }

        takeResultLauncher = registerForActivityResult(ActivityResultContracts.StartActivityForResult(),
            ActivityResultCallback { result ->
                if (result.resultCode == RESULT_OK) {
                    Toast.makeText(applicationContext, "Bluetooth enabled", Toast.LENGTH_SHORT).show()
                } else {
                    Toast.makeText(applicationContext, "Bluetooth disabled", Toast.LENGTH_SHORT).show()
                }
            })

        binding.btnBluetoothOn.setOnClickListener {
            takePermission.launch(Manifest.permission.BLUETOOTH_CONNECT)
        }

        binding.btnBluetoothOff.setOnClickListener {
            if (ActivityCompat.checkSelfPermission(applicationContext, Manifest.permission.BLUETOOTH_CONNECT) == PackageManager.PERMISSION_GRANTED) {
                bluetoothAdapter.disable()
                Toast.makeText(applicationContext, "Bluetooth Disabled", Toast.LENGTH_SHORT).show()
            }
        }

        binding.btnBluetoothPaired.setOnClickListener {
            val data = StringBuffer()
            val pairedDevices = bluetoothAdapter.bondedDevices
            for (device in pairedDevices) {
                data.append("Device name: ${device.name}\nDevice address: ${device.address}\n")
                if (device.address == "20:19:07:00:4A:0F") {
                    myBltDevice = device
                }
            }
            if (data.isEmpty()) {
                Toast.makeText(applicationContext, "Paired devices are not found", Toast.LENGTH_LONG).show()
            } else {
                if (::myBltDevice.isInitialized && (myBltDevice.address == "20:19:07:00:4A:0F")) {
                    Toast.makeText(applicationContext, "${myBltDevice.name}\n${myBltDevice.address}", Toast.LENGTH_SHORT).show()
                } else {
                    Toast.makeText(applicationContext, "You have paired devices, but HC is not paired", Toast.LENGTH_SHORT).show()
                }
            }
        }

        binding.btnConnect.setOnClickListener {
            connectToDevice()
        }

        binding.btnSend.setOnClickListener {
            sendCommand("b")
        }
        binding.btnSend2.setOnClickListener {
            sendCommand("c")
        }
        binding.btnkitchen.setOnClickListener {
            val explicitIntent = Intent(applicationContext, KitchenActivity::class.java)
            explicitIntent.putExtra("KEY", language)
            startActivity(explicitIntent)
        }
        binding.btnlivingRoom.setOnClickListener {
            val explicitIntent = Intent(applicationContext, LivingRoomActivity::class.java)
            explicitIntent.putExtra("KEY", language)
            startActivity(explicitIntent)
        }
        binding.buttonLanguage.setOnClickListener {
            if(language == 0)
            {
                binding.btnBluetoothOn.text="Bluetooth Wł"
                binding.btnBluetoothOff.text="Bluetooth Wył"
                binding.btnSend.text="Wiadomość testowa"
                binding.btnSend2.text="Wiadomość testowa 2"
                binding.buttonLanguage.text="Język"
                binding.btnBluetoothPaired.text="Sprawdź połączenie z HC"
                binding.btnConnect.text = "Polacz sie z modulem HC05"
                binding.btnkitchen.text = "Kuchnia"
                binding.btnlivingRoom.text = "Salon"
                language = 1;
            }
            else if(language == 1)
            {
                binding.btnBluetoothOn.text="Bluetooth ON"
                binding.btnBluetoothOff.text= "Bluetooth OFF"
                binding.btnSend.text= "Test Message"
                binding.btnSend2.text= "Test message 2"
                binding.buttonLanguage.text="Language"
                binding.btnBluetoothPaired.text="Check if HC is paired"
                binding.btnConnect.text = "Connect to HC module"
                binding.btnkitchen.text = "Kitchen"
                binding.btnlivingRoom.text = "Living Room"
                language = 0;
            }
        }
    }

    private fun connectToDevice() {
        if (::myBltDevice.isInitialized) {
            thread {
                try {
                    if (ActivityCompat.checkSelfPermission(
                            this,
                            Manifest.permission.BLUETOOTH_CONNECT
                        ) != PackageManager.PERMISSION_GRANTED
                    ) {
                        return@thread
                    }
                    btSocket = myBltDevice.createRfcommSocketToServiceRecord(MY_UUID)
                    btSocket?.connect()
                    BluetoothConnectionManager.btSocket = btSocket // Assign the socket to the singleton
                    runOnUiThread {
                        Toast.makeText(this, "Connected to HC module", Toast.LENGTH_SHORT).show()
                    }
                } catch (e: IOException) {
                    e.printStackTrace()
                    runOnUiThread {
                        Toast.makeText(this, "Can't Connect to HC module", Toast.LENGTH_SHORT).show()
                    }
                    try {
                        btSocket?.close()
                    } catch (closeException: IOException) {
                        closeException.printStackTrace()
                    }
                }
            }
        } else {
            Toast.makeText(this, "No device selected", Toast.LENGTH_SHORT).show()
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
        }
    }
}

