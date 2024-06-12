package com.alexisboiz.wallwatcher

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.EditText
import android.widget.TextView
import com.google.android.material.snackbar.Snackbar
import com.google.firebase.auth.ktx.auth
import com.google.firebase.ktx.Firebase

class Signup : Fragment() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.signup, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        val email = view.findViewById<EditText>(R.id.emailEditText)
        val password = view.findViewById<EditText>(R.id.passwordEditText)
        val submit = view.findViewById<Button>(R.id.signupButton)
        submit.setOnClickListener{
            val emailText = email.text.toString()
            val passwordText = password.text.toString()
            if(emailText.isNotEmpty() && passwordText.isNotEmpty()){
                Firebase.auth.createUserWithEmailAndPassword(emailText, passwordText)
                    .addOnCompleteListener { task ->
                        if (task.isSuccessful) {
                            val fragmentTransaction = parentFragmentManager.beginTransaction()
                            fragmentTransaction.replace(R.id.frameLayout, Home())
                            fragmentTransaction.commit()
                        }else{
                            Snackbar.make(view, "Error: ${task.exception?.message}", Snackbar.LENGTH_LONG).show()
                        }
                    }
            }

        }

    }
}