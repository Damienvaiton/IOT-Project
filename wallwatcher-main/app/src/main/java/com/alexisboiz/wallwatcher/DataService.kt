package com.alexisboiz.wallwatcher

import com.alexisboiz.wallwatcher.model.Distance
import retrofit2.Response
import retrofit2.http.Field
import retrofit2.http.FormUrlEncoded
import retrofit2.http.GET
import retrofit2.http.POST
import retrofit2.http.Path
import retrofit2.http.Query

interface DataService {
    @GET("/distance")
    suspend fun getDistance() : Response<String>

    @POST("/set-tone-frequency")
    suspend fun setToneFrequency(
        @Query("tone") tone : String,
        @Query("frequency") frequency : String,
        @Query("id") id : String)
    : Response<String>
}