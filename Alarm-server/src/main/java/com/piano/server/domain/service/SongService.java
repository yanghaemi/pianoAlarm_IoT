package com.piano.server.domain.service;

import java.util.List;
import java.util.Optional;

import org.springframework.stereotype.Service;
import lombok.RequiredArgsConstructor;

import com.piano.server.domain.DTO.*;
import com.piano.server.global.response.ApiResponse;
import com.piano.server.domain.repository.SongRepository;
import com.piano.server.domain.entity.SongEntity;

@Service
@RequiredArgsConstructor
public class SongService {

    private final SongRepository songRepository;

    public ApiResponse<SaveSongResponse> saveSong(SaveSongRequest request) {
        // title과 notes를 DB에 저장하는 기능
        // method: POST
        ApiResponse<SaveSongResponse> response;
        SaveSongResponse data;
        try {

            SongEntity entity = SongEntity.builder()
                    .title(request.getTitle())
                    .notes(request.getNotes())
                    .build();

            songRepository.save(entity);

            data = new SaveSongResponse(entity.getId(), entity.getTitle(), entity.getNotes());

            response = new ApiResponse<>(200, data, "노래 저장 성공");
        } catch (Exception e) {
            response = new ApiResponse<>(500, null, "노래 저장 실패: " + e.getMessage());
        }

        return response;

    }

    public ApiResponse<List<GetSongListResponse>> getSongList() {
        ApiResponse<List<GetSongListResponse>> response;
        List<GetSongListResponse> data = null;

        try {

            // 엔티티 가져오기
            List<SongEntity> entities = songRepository.findAll();

            // 엔티티 리스트 -> dto 리스트로 변환
            data = entities.stream()
                    .map(e -> new GetSongListResponse(
                            e.getId(),
                            e.getTitle(),
                            e.getNotes()))
                    .toList();

            response = new ApiResponse<>(200, data, "노래 리스트 가져오기 성공");
        } catch (Exception e) {
            response = new ApiResponse<>(500, null, "노래 리스트 가져오기 실패: " + e.getMessage());
        }

        return response;
    }

    public ApiResponse<GetSongResponse> getSong(GetSongRequest request) {
        ApiResponse<GetSongResponse> response;
        GetSongResponse data;

        try {

            Long id = request.getId();
            // 엔티티 가져오기
            // SongEntity entity = songRepository.findById(request.getId());
            Optional<SongEntity> optionalEntity = songRepository.findById(id);

            if (optionalEntity.isPresent()) {
                SongEntity entity = optionalEntity.get();
                data = new GetSongResponse(
                        entity.getId(),
                        entity.getTitle(),
                        entity.getNotes());

                response = new ApiResponse<>(200, data, "노래 정보 가져오기 성공");
            } else
                response = new ApiResponse<>(400, null, "해당 노래를 찾을 수 없습니다.");

        } catch (Exception e) {
            response = new ApiResponse<>(500, null, "노래 정보 가져오기 실패: " + e.getMessage());
        }
        return response;
    }

    public ApiResponse<DeleteSongResponse> deleteSong(Long id) {
        ApiResponse<DeleteSongResponse> response;
        DeleteSongResponse data = null;

        try {

            Optional<SongEntity> optionalEntity = songRepository.findById(id);

            if (optionalEntity.isPresent()) {
                SongEntity entity = optionalEntity.get();

                data = new DeleteSongResponse(
                        entity.getId(),
                        entity.getTitle(),
                        entity.getNotes());

                songRepository.deleteById(id);

                response = new ApiResponse<>(200, data, "노래 삭제 성공");

            } else
                response = new ApiResponse<>(400, null, "해당 노래를 찾을 수 없습니다.");

        } catch (Exception e) {
            response = new ApiResponse<>(500, null, "노래 삭제 실패: " + e.getMessage());
        }
        return response;
    }
}
