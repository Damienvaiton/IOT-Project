package com.alexisboiz.boursewatcher.domain.datasource

import android.util.Log
import com.alexisboiz.wallwatcher.DataService
import okhttp3.OkHttpClient
import okhttp3.logging.HttpLoggingInterceptor
import retrofit2.Retrofit
import retrofit2.converter.gson.GsonConverterFactory

object NetworkDataSource {
    private const val BASE_URL = "https://api-wallwatcher.onrender.com"

    private val logInterceptor = HttpLoggingInterceptor(){
        Log.d("Okhttp", it)
    }.apply{
        level = HttpLoggingInterceptor.Level.BODY
    }

    private val okHttp = OkHttpClient.Builder()
        .addNetworkInterceptor(logInterceptor)
        .build()

    private val retrofit =
        Retrofit.Builder()
            .client(okHttp)
            .baseUrl(BASE_URL)
            .addConverterFactory(GsonConverterFactory.create())
            .build()


    val dataService: DataService = retrofit.create(DataService::class.java)
}