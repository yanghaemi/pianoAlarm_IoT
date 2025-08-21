'use client'

import Image from 'next/image'
import React, { useState } from 'react';
import axios from 'axios';
import 'bootstrap/dist/css/bootstrap.min.css';
import './globals.css'

export default function Page() {

  const [title, setTitle] = useState("");
  const [currentSong, setCurrentSong] = useState("");
  const [song, setSong] = useState([]); // 노래
  const [songlist, setSongList] = useState([]); 
  const [songFlag, setSongFlag] = useState(false); // 노래 녹음 플래그
  const [apiUrl, setApiUrl] = useState("http://localhost:8080");

  const saveSong = async () => {
    
      try {
        const response = await axios.post(`${apiUrl}/api/savesong`, {
          "title": title,
          "notes" : song
        })

        console.log("응답: " + response.data);
      } catch (e) {
        console.log("에러: " + e);
      }
  };

  const playSong = async() => {
    try {
      const response = await axios.post(`${apiUrl}/esp/playsong`, {
        "notes" : song
      })

      console.log("응답: " + response);
    } catch (e){
      console.log("에러: " + e);
    }
  }

  const setAlram = async () => {
    try {
      const response = await axios.post(`${apiUrl}/esp/setSong`,
        {
          "notes": song
        })
      
      console.log("응답: " + response);
    } catch (e) {
      console.log("에러: " + e);
    }
  };
  
  return (
    <>
      <div className="piano">
      <div className="white-keys">
        {['C', 'D', 'E', 'F', 'G', 'A', 'B'].map((note) => (
          <button className="btn btn-outline-dark key white" key={note}>
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
      
      <div className="btns">
        <button className="btn btn-dark normal" onClick={() => setSongFlag(true) }>녹음 시작</button>
        <button className="btn btn-dark normal" onClick={() => setSongFlag(false) }>녹음 종료</button>
        <button className="btn btn-dark normal" onClick={saveSong} >저장</button>
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
      </div>
      
      <div className="listBox">
        <div className="songList">
          <h4>저장된 노래</h4>
        </div>
        
        <text className='normal'>현재 곡 : {currentSong} </text>
          
        <button className="btn btn-secondary playBtn normal" onClick={playSong}>재생</button>
        <button className="btn btn-secondary setBtn normal" onClick={setAlram}>이 곡으로 알람 설정</button>
        
      </div>

      <div className="alarmBox">
        <div className=''>
          <h4>알람 설정</h4>
        </div>
      </div>
    </>
  );
}
