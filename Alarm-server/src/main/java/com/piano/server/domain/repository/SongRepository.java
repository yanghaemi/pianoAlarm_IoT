package com.piano.server.domain.repository;

import org.springframework.data.jpa.repository.JpaRepository;

import com.piano.server.domain.entity.SongEntity;

public interface SongRepository extends JpaRepository<SongEntity, Long> {

}
