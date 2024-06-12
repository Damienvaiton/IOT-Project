package com.alexisboiz.wallwatcher

import com.alexisboiz.boursewatcher.domain.datasource.NetworkDataSource
import com.alexisboiz.wallwatcher.model.Distance
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.flow
import retrofit2.Response

object DataRepository {
    fun getDistance() : Flow<Response<String>> = flow{
        emit(NetworkDataSource.dataService.getDistance())
    }

    fun setToneFrequency(tone : String, frequency : String, id : String) : Flow<Response<String>> = flow{
        emit(NetworkDataSource.dataService.setToneFrequency(tone,frequency,id))
    }
}