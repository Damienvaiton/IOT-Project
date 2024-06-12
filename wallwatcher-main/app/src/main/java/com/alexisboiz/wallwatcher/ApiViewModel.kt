package com.alexisboiz.wallwatcher

import android.util.Log
import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.flow.catch
import kotlinx.coroutines.launch
import com.alexisboiz.boursewatcher.domain.datasource.NetworkDataSource
import com.alexisboiz.wallwatcher.model.Distance
import kotlinx.coroutines.flow.collect


class ApiViewModel : ViewModel() {
    private var _distanceLiveData : MutableLiveData<Int> = MutableLiveData()
    val distanceLiveData : LiveData<Int> = _distanceLiveData

    private var _buzzerLiveData : MutableLiveData<String> = MutableLiveData()
    val buzzerLiveData : LiveData<String> = _buzzerLiveData

    fun fetchDistance(){
        viewModelScope.launch {
            DataRepository.getDistance()
                .catch {
                    Log.e("ApiViewModel", "fetchDistance: ${it.message}")
                }
                .collect(){
                    _distanceLiveData.postValue(it.body()?.toInt())
                }
        }
    }

    fun setToneFrequency(tone : String, frequency : String, id : String){
        viewModelScope.launch {
            DataRepository.setToneFrequency(tone, frequency, id)
                .catch {
                    Log.e("ApiViewModel", "setToneFrequency: ${it.message}")
                }
                .collect{
                    _buzzerLiveData.postValue(it.body())
                }
        }
    }
}