'use client'

import React, { useEffect, useState } from 'react';
import axios from 'axios';
import 'bootstrap/dist/css/bootstrap.min.css';
import './globals.css'

export default function Page() {

  type Song = {
    id: number;
    title: string;
    notes: string;
  };

  const [title, setTitle] = useState("");
  const [currentSong, setCurrentSong] = useState<Song | null>(null);
  const [song, setSong] = useState(""); // 노래
  const [songList, setSongList] = useState<Song[]>([]); // 노래 리스트
  const [result, setResult] = useState("");
  const apiUrl = process.env.NEXT_PUBLIC_API_URL;
  const esp32Url = process.env.NEXT_PUBLIC_ESP_URL;
  const [alarmHour, setAlarmHour] = useState<number>(0);
  const [alarmMin, setAlarmMin] = useState<number>(0);
  const [currentHour, setCurrentHour] = useState<number>(0);
  const [currentMin, setCurrentMin] = useState<number>(0);

  useEffect(() => {
    getSongList();
    const interval = setInterval(() => {
      getSongList();
    }, 5000);

    return () => clearInterval(interval);

  },[]);

  const getSongList = async () => {

    try {
      const response = await axios.get(`${apiUrl}/api/getsonglist`);

      setSongList(response.data.data);

      console.log("응답: " + response.data.data);
    } catch (e) {
      console.log("응답: " + e);
    }
  };

  const saveSong = async () => {

    console.log("제목: " + title);
    console.log("notes: " + song);

      try {
        const response = await axios.post(`${apiUrl}/api/savesong`, {
          "title": title,
          "notes": song
        });
        setResult("노래 저장 완료 😺");
        console.log("응답: " + response.data);
      } catch (e) {
        setResult("노래 저장 실패 😱"+e);

        console.log("에러: " + e);
      }
  };

  const playSong = async() => {
    try {
      const mySong: Song = {
        id: 1,
        title: "test",
        notes: "CDEFGABC"
      };

      setCurrentSong(mySong);

      const response = await axios.post(`${esp32Url}/playsong`, {
        "notes" : currentSong?.notes
      })
      console.log("playSong 보낼 거 : " + currentSong?.notes);

      console.log("응답: " + response);
    } catch (e){
      console.log("에러: " + e);
    }
  }

  const setAlarm = async () => {
    
    try {
      const response = await axios.post(`${esp32Url}/setsong`,
        {
          "notes": currentSong?.notes
        });
      console.log("setAlarm 보낼 거 : " + currentSong?.notes);
      console.log("응답: " + response);
    } catch (e) {
      console.log("에러: " + e);
    }
  };

  const setCurrentTime = async () => {
    try {
      const response = await axios.post(`${esp32Url}/setcurrenttime`, {
        "hour": currentHour,
        "min": currentMin
      })
      console.log("응답: " + response.data);
    } catch (e) {
      console.log("에러: " + e);
    }
  };

  const setAlarmTime = async () => {
    try {
      const response = await axios.post(`${esp32Url}/setalarmtime`, {
        "hour": alarmHour,
        "min" : alarmMin
      });

      console.log("응답: " + response);
      setResult("알람 노래로 설정 😺 \n"+currentSong?.title + "\n"+currentSong?.notes);
    } catch (e) {
      console.log("error: " + e);
      setResult("알람 노래 설정 실패 😱");
    }
  };

  const deleteSong = async (song: { id: number; title: String; notes: String; }) => {
    try {
      const response = await axios.delete(`${apiUrl}/api/deletesong`,
        {
          params: { id: song.id }
        });
      console.log("노래 삭제 😺: " + response);
    } catch (e) {
      console.log("노래 삭제 실패 😱: " + e);
    }
  }

  return (
    <>
      {/* 노래 녹음 및 저장 */}
      <div className="btns">
        <button className="btn btn-dark normal"
          onClick={() => {
            setResult("");
            setSong(""); // 노래 초기화
            setTitle("");
          }}
        >초기화</button>
        <input    // 제목 입력
          type='text'
          value={title}
          onChange={(e) => {
            const newTitle = e.target.value;
            setTitle(newTitle);
            console.log(newTitle);
          }}
          placeholder='노래 제목을 입력해주세요.'
        />
        <button
          className="btn btn-dark normal"
          onClick={() => {
           
            if (!title) {
              setResult("❗제목을 입력하세요.");
              console.log("제목을 입력하세요.");
            }
            if (!song) {
              setResult(result + "❗노래를 입력하세요.");
              console.log("노래를 입력하세요.");
            }
            if (title && song) {
             
              saveSong();
            }
          }}
        >저장</button>
        <text>🐰 {song} {result}</text>
      </div>
      {/* 피아노 건반 */}
      <div className="piano">
      <div className="white-keys">
        {['C', 'D', 'E', 'F', 'G', 'A', 'B'].map((note) => (
          <button className="btn btn-outline-dark key white"
            key={note}
            onClick={() => {
              setSong(song+note);
              console.log(song);
            }}
          >
            {note}
          </button>
        ))}
      </div>
      <div className="black-keys">
        <button className="btn btn-dark key black">C#</button>
        <div className="spacer" />
        <button className="btn btn-dark key black">D#</button>
        <div className="wide-spacer spacer" /> 
        <button className="btn btn-dark key black">F#</button>
        <div className="spacer" />
        <button className="btn btn-dark key black">G#</button>
        <div className="spacer" />
        <button className="btn btn-dark key black">A#</button>
        <div className="spacer" /> 
      </div>
      </div>
      {/* 노래 리스트 */}
      <div className="listBox">
        <button className="btn btn-secondary renewBtn normal" onClick={getSongList}>갱신</button>
        <div className="songList">
          <div className='songListTop'>
            <h4>저장된 노래</h4>
          </div>
          
          <div className='songListBody'>
            {songList.map((song) => (
              <div className='songEntity' key={song.id} >
                <button className="btn song" onClick={() => { setCurrentSong(song); }}>
                  {song.title}
                </button>
                <button
                  className='btn deletebtn'
                  onClick={() => {
                    deleteSong(song);
                  }}>🗑️
                </button>
              </div>
            ))}
          </div> 
        </div>
        
        <text className='normal'>현재 곡 : {currentSong?.title} <br/> {result} </text>
          
        <button className="btn btn-secondary playBtn normal" onClick={playSong}>재생</button>
        <button className="btn btn-secondary setBtn normal" onClick={setAlarm}>이 곡으로 알람 설정</button>
      </div>

      {/* 알람 */}
      <div className="alarmBox">
          <h4>현재 시간 설정 {currentHour} : {currentMin}</h4>
          <div className="input-group mb-3">
            <input
              type="number"
              className="form-control"
              placeholder="hour"
              aria-label="setTime"
              value={currentHour}
              onChange={(e) => setCurrentHour(parseInt(e.target.value, 10))}
              aria-describedby="button-addon2" />
            <span className="input-group-text">:</span>
            <input
              type="number"
              className="form-control"
              placeholder="min"
              aria-label="setTime"
              value={currentMin}
              onChange={(e) => setCurrentMin(parseInt(e.target.value, 10))}
            aria-describedby="button-addon2" />
            <button
              className="btn btn-outline-secondary"
              type="button"
              id="button-addon2"
              onClick={() => {
                console.log(`현재 시각: ${currentHour}시 ${currentMin}분`);
                setCurrentTime();
              }}>현재 시간 설정
            </button>
          </div>
      </div>
      <div className="alarmBox">
        <h4>알람 시간 설정 {alarmHour} : {alarmMin}</h4>
          <div className="input-group mb-3">
            <input
              type="number"
              className="form-control"
              placeholder="hour"
              aria-label="setTime"
              value={alarmHour}
              onChange={(e) => setAlarmHour(parseInt(e.target.value, 10))}
              aria-describedby="button-addon2" />
            <span className="input-group-text">:</span>
            <input
              type="number"
              className="form-control"
              placeholder="min"
              aria-label="setTime"
              value={alarmMin}
              onChange={(e) => setAlarmMin(parseInt(e.target.value, 10))}
            aria-describedby="button-addon2" />
            <button
              className="btn btn-outline-secondary"
              type="button"
              id="button-addon2"
              onClick={() => {
                console.log(`알람: ${alarmHour}시 ${alarmMin}분`);
                setAlarmTime();
              }}>알람 시간 설정 
            </button>
          </div>
        </div>
    </>
  );
}
