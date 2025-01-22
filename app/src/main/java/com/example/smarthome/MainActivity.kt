package com.example.smarthome

import android.Manifest
import android.annotation.SuppressLint
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothManager
import android.bluetooth.BluetoothSocket
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Build
import android.os.Bundle
import android.widget.Toast
import androidx.activity.result.ActivityResultCallback
import androidx.activity.result.ActivityResultLauncher
import androidx.activity.result.contract.ActivityResultContracts
import androidx.annotation.RequiresApi
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import com.example.smarthome.databinding.ActivityMainBinding
import java.io.IOException
import java.util.*
import kotlin.concurrent.thread

class MainActivity : AppCompatActivity() {

    private lateinit var myBltDevice: BluetoothDevice
    private lateinit var binding: ActivityMainBinding
    private lateinit var bluetoothManager: BluetoothManager
    private lateinit var bluetoothAdapter: BluetoothAdapter
    private lateinit var takePermission: ActivityResultLauncher<String>
    private lateinit var takeResultLauncher: ActivityResultLauncher<Intent>
    private val MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB")
    private var btSocket: BluetoothSocket? = null

    @RequiresApi(Build.VERSION_CODES.S)
    @SuppressLint("SuspiciousIndentation")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        bluetoothManager = getSystemService(BLUETOOTH_SERVICE) as BluetoothManager
        bluetoothAdapter = bluetoothManager.adapter

        //Dostanie permisji do połączenia
        takePermission = registerForActivityResult(ActivityResultContracts.RequestPermission()) {
            if (it) {
                val intent = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
                takeResultLauncher.launch(intent)
            } else {
                Toast.makeText(
                    applicationContext,
                    "Bluetooth Permission is not Granted",
                    Toast.LENGTH_SHORT
                ).show()
            }
        }

        //Łączenie z bluetoothem
        takeResultLauncher = registerForActivityResult(ActivityResultContracts.StartActivityForResult(),
                ActivityResultCallback { result ->
                    if (result.resultCode == RESULT_OK) {
                        Toast.makeText(applicationContext, "Bluetooth enabled", Toast.LENGTH_SHORT)
                            .show()
                    } else {
                        Toast.makeText(applicationContext, "Bluetooth disabled", Toast.LENGTH_SHORT)
                            .show()
                    }
                })

        //Włączenie bluetootha
        binding.btnBluetoothOn.setOnClickListener {
            takePermission.launch(Manifest.permission.BLUETOOTH_CONNECT)
        }

        //Wyłączenie bluetootha
        binding.btnBluetoothOff.setOnClickListener {
            if (ActivityCompat.checkSelfPermission(
                    applicationContext,
                    Manifest.permission.BLUETOOTH_CONNECT
                ) == PackageManager.PERMISSION_GRANTED
            ) {
                bluetoothAdapter.disable()
                Toast.makeText(applicationContext, "Bluetooth Disabled", Toast.LENGTH_SHORT).show()
            }
        }

        //Sprawdzenie czy moduł znajduje się na liście urządzeń połączonych
        binding.btnBluetoothPaired.setOnClickListener {
            val data = StringBuffer()
            val pairedDevices = bluetoothAdapter.bondedDevices
            for (device in pairedDevices) {
                data.append("Device name: ${device.name}\nDevice address: ${device.address}\n")
                if (device.address == "58:56:00:00:82:B2") { //00:23:00:01:64:84
                    myBltDevice = device
                }
            }
            if (data.isEmpty()) {
                Toast.makeText(
                    applicationContext,
                    "Paired devices are not found",
                    Toast.LENGTH_LONG
                ).show()
            } else {
                if (::myBltDevice.isInitialized && (myBltDevice.address == "58:56:00:00:82:B2")) {
                    Toast.makeText(
                        applicationContext,
                        "${myBltDevice.name}\n${myBltDevice.address}",
                        Toast.LENGTH_SHORT
                    ).show()
                } else {
                    Toast.makeText(
                        applicationContext,
                        "You have paired devices, but HC is not paired",
                        Toast.LENGTH_SHORT
                    ).show()
                }
            }
        }

        //Wywołanie połączenia
        binding.btnConnect.setOnClickListener {
            connectToDevice()
        }

        //Przejście do funkcji
        binding.btnFunctions.setOnClickListener {
            val explicitIntent = Intent(applicationContext, FunctionsActivity::class.java)
            startActivity(explicitIntent)
        }

        //Przejście do panelu fotowoltaicznego
        binding.btnPV.setOnClickListener {
            val explicitIntent = Intent(applicationContext, SolarPanelActivity::class.java)
            startActivity(explicitIntent)
        }

    }

    //Funkcja służąca do łączenia telefonu z modułem bluetooth
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
                    BluetoothCommunicationManager.btSocket = btSocket
                    runOnUiThread {
                        Toast.makeText(this, "Connected to HC module", Toast.LENGTH_SHORT).show()
                    }
                } catch (e: IOException) {
                    e.printStackTrace()
                    runOnUiThread {
                        Toast.makeText(this, "Can't Connect to HC module", Toast.LENGTH_SHORT)
                            .show()
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
}



