'use client'

import Image from 'next/image'
import React, { useEffect, useState } from 'react';
import axios from 'axios';
import 'bootstrap/dist/css/bootstrap.min.css';
import './globals.css'

export default function Page() {

  type Song = {
    id: number;
    title: String;
    notes: String;
  };

  const [title, setTitle] = useState("");
  const [currentSong, setCurrentSong] = useState<Song | null>(null);
  const [song, setSong] = useState(""); // 노래
  const [songList, setSongList] = useState<Song[]>([]); // 노래 리스트
  const [songFlag, setSongFlag] = useState(false); // 노래 녹음 플래그
  const [result, setResult] = useState("");
  const apiUrl = "http://localhost:8080";
  const esp32Url = "http://ddd";

  useEffect(() => {
    getSongList();
  },[]);

  const getSongList = async () => {

    try {
      const response = await axios.get(`${apiUrl}/api/getsonglist`);

      setSongList(response.data.data);

      console.log("응답: " + response.data.data);
      console.log("곡 리스트: " + response.data.data);
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

  const deleteSong = async (song) => {
    try {
      const response = await axios.delete(`${apiUrl}/api/deletesong`,
        {
          params: { id: song.id }
        });

      console.log("노래 삭제: " + response);
    } catch (e) {
      console.log("노래 삭제 에러: " + e);
    }
  }

  
  return (
    <>
      {/* 피아노 건반 */}
      <div className="piano">
      <div className="white-keys">
        {['C', 'D', 'E', 'F', 'G', 'A', 'B'].map((note) => (
          <button className="btn btn-outline-dark key white"
            key={note}
            onClick={() => {
              if (songFlag) {
                setSong(song ? song + ", " + note : note);
                console.log(song);
              }
              else {
                setResult("녹음 시작을 눌러주세요!");
                console.log("녹음 상태가 아닙니다.");
              }
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
      
      {/* 노래 녹음 및 저장 */}
      <div className="btns">
        <button className="btn btn-dark normal" onClick={() => {
          setSongFlag(true);
          setResult("");
          setSong(""); // 노래 초기화
          setTitle("");
          }}
        >녹음 시작</button>
        <button
          className="btn btn-dark normal"
          onClick={() => {
          setSongFlag(false);
        }}
        >녹음 종료</button>
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
              setResult("❗제목을 입력하세요.")
              console.log("제목을 입력하세요.");
            }
            if (!song) {
              setResult(result + "❗노래를 입력하세요.")
              console.log("노래를 입력하세요.");
            }
            if (title && song) {
              setResult("노래 저장 완료 😺");
              saveSong();
              getSongList();  // 리스트 갱신
            }
          }}
        >저장</button>
        <text>{songFlag ? "녹음 중 ": ""}🐰 {song} {result}</text>
      </div>
      
      {/* 노래 리스트 */}
      <div className="listBox">
        <div className="songList">
          <div className='songListTop'>
            <h4>저장된 노래</h4><button className="btn btn-secondary renewBtn normal" onClick={getSongList}>갱신</button>
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
                    getSongList();  // 리스트 갱신
                  }}>🗑️
                </button>
              </div>
            ))}
          </div> 
        </div>
        
        <text className='normal'>현재 곡 : {currentSong?.title} </text>
          
        <button className="btn btn-secondary playBtn normal" onClick={playSong}>재생</button>
        <button className="btn btn-secondary setBtn normal" onClick={setAlram}>이 곡으로 알람 설정</button>
      </div>

      {/* 알람 */}
      <div className="alarmBox">
        <div className=''>
          <h4>알람 설정</h4>
        </div>
      </div>
    </>
  );
}
