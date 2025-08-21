package com.piano.server.domain.controller;

import java.util.List;

import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

import com.piano.server.domain.DTO.*;
import com.piano.server.domain.service.SongService;
import com.piano.server.global.response.ApiResponse;

import lombok.RequiredArgsConstructor;;

@RestController
@RequiredArgsConstructor
public class SongController {

    private final SongService service;

    /*
     * index
     */
    @GetMapping("/")
    public String home() {
        return "Piano API up 🎹";
    }

    /*
     * 메서드: POST
     * api url: http://localhost:8080/api/savesong
     * 설명: 노래를 DB에 저장합니다.
     */
    @PostMapping("/api/savesong")
    public ApiResponse<SaveSongResponse> saveSong(@RequestBody SaveSongRequest request) {
        ApiResponse<SaveSongResponse> result = service.saveSong(request);
        System.out.println("save the song : " + result);

        return result;
    }

    /*
     * 메서드: GET
     * api url: http://localhost:8080/api/getsonglist
     * 설명: DB에 저장된 노래 리스트를 가져옵니다.
     */
    @GetMapping("/api/getsonglist")
    public ApiResponse<List<GetSongListResponse>> getSongList() {

        ApiResponse<List<GetSongListResponse>> result = service.getSongList();
        System.out.println("get the song list : " + result);

        return result;
    }

    /*
     * 메서드: GET
     * api url: http://localhost:8080/api/getsong
     * 설명: DB에 저장된 임의의 노래 악보를 가져옵니다.
     */
    @GetMapping("/api/getsong")
    public ApiResponse<GetSongResponse> getSong(@RequestParam GetSongRequest request) {

        ApiResponse<GetSongResponse> result = service.getSong(request);
        System.out.println("get the song : " + result);

        return result;
    }

    /*
     * 메서드: DELETE
     * api url: http://localhost:8080/api/deletesong
     * 설명: DB에 저장된 임의의 노래를 삭제합니다.
     */
    @DeleteMapping("/api/deletesong")
    public ApiResponse<DeleteSongResponse> deleteSong(@RequestParam Long id) {

        ApiResponse<DeleteSongResponse> result = service.deleteSong(id);
        System.out.println("get the song : " + result);

        return result;
    }
}
