package com.example.smarthome

import android.os.Bundle
import android.util.TypedValue
import android.widget.Toast
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import com.example.smarthome.databinding.ActivityAlarmBinding
import android.view.View
import android.widget.LinearLayout

class AlarmActivity : AppCompatActivity() {
    private lateinit var binding: ActivityAlarmBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityAlarmBinding.inflate(layoutInflater)
        setContentView(binding.root)
        enableEdgeToEdge()

        // Rejestracja listenera Bluetooth
        BluetoothCommunicationManager.registerListener(object :
            BluetoothCommunicationManager.BluetoothDataListener {
            override fun onDataReceived(data: String)
            {}

            override fun onError(error: String) {
                // Wyświetlanie błędu, jeśli wystąpił problem z Bluetooth
                Toast.makeText(this@AlarmActivity,  error, Toast.LENGTH_SHORT)
                    .show()
            }
        })

        // Obsługa przełącznika do alarmu
        binding.switchAlarm.setOnCheckedChangeListener { _, isChecked ->
            val command = if (isChecked) "AL01" else "AL00" // Wysyłamy odpowiednią komendę w zależności od stanu przełącznika
            BluetoothCommunicationManager.sendData(command) // Wysyłanie komendy przez BluetoothCommunicationManager
        }

        // Obsługa przełącznika do otwierania drzwi
        binding.switch2.setOnCheckedChangeListener { _, isChecked ->
            val command = if (isChecked) "DM01" else "DM00"
            BluetoothCommunicationManager.sendData(command)
        }

        // Obsługa przełącznika do otwierania drzwi
        binding.switch3.setOnCheckedChangeListener { _, isChecked ->
            val command = if (isChecked) "GM01" else "GM00"
            BluetoothCommunicationManager.sendData(command)
        }

        // Obsługa przycisku do sprawdzania hasła
        binding.buttonCheckPassword.setOnClickListener {
            val enteredText = binding.ET.text.toString()
            if (enteredText == "1234") {  // Jeśli hasło jest poprawne, pokazujemy odpowiedni komunikat i zmieniamy UI
                Toast.makeText(applicationContext, "Password is correct", Toast.LENGTH_SHORT).show()
                val params = binding.LayoutAlarm.layoutParams as LinearLayout.LayoutParams
                params.bottomMargin = TypedValue.applyDimension(
                    TypedValue.COMPLEX_UNIT_DIP,
                    40f,
                    resources.displayMetrics
                ).toInt()
                binding.LayoutAlarm.layoutParams = params
                binding.Text1.visibility = View.VISIBLE
                binding.Layout1.visibility = View.VISIBLE
                binding.Text2.visibility = View.VISIBLE
                binding.Layout2.visibility = View.VISIBLE
                binding.Text3.visibility = View.VISIBLE
                binding.Layout3.visibility = View.VISIBLE
            } else {
                Toast.makeText(applicationContext, "Password is incorrect", Toast.LENGTH_SHORT)
                    .show()
            }
        }
    }

    // Metoda wywoływana przy zniszczeniu Activity
    override fun onDestroy() {
        super.onDestroy()
        // Wyrejestrowanie listenera po zniszczeniu Activity
        BluetoothCommunicationManager.unregisterListener(object :
            BluetoothCommunicationManager.BluetoothDataListener {
            override fun onDataReceived(data: String) {}
            override fun onError(error: String) {}
        })
    }
}

