package com.alexisboiz.wallwatcher

import android.os.Bundle
import android.util.Log
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.webkit.WebView
import android.webkit.WebViewClient
import android.widget.TextView
import androidx.fragment.app.viewModels
import com.alexisboiz.wallwatcher.model.Distance
import com.google.firebase.database.DataSnapshot
import com.google.firebase.database.DatabaseError
import com.google.firebase.database.FirebaseDatabase
import com.google.firebase.database.ValueEventListener

class Camera : Fragment() {
    val apiViewModel by viewModels<ApiViewModel>()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        val webView = view.findViewById<WebView>(R.id.webview)
        val distance = view.findViewById<TextView>(R.id.distance)

        webView.webViewClient = WebViewClient()

        webView.loadUrl("http://192.168.196.70:81/stream")
        webView.settings.javaScriptEnabled = true

        webView.settings.loadsImagesAutomatically = true

        val ref = FirebaseDatabase.getInstance().getReference("/000000001/distance")
        ref.addValueEventListener(object : ValueEventListener {
            override fun onDataChange(dataSnapshot: DataSnapshot) {
                val value = dataSnapshot.getValue(Distance::class.java)
                if (value != null) {
                    apiViewModel.distanceLiveData.observe(viewLifecycleOwner){
                        val frequency : String = (it?.times(10)).toString()
                        Log.e("Main", frequency)
                        val note = "NOTE_C5"
                        apiViewModel.buzzerLiveData.observe(viewLifecycleOwner){
                            Log.e("Main",it.toString())
                        }
                        apiViewModel.setToneFrequency(note, frequency, "000000001")
                        distance.text = "${it} M"
                    }
                    apiViewModel.fetchDistance()
                }
            }
            override fun onCancelled(error: DatabaseError) {
                Log.e("Distance","Failed to read value.")
            }
        })

    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_camera, container, false)
    }
}