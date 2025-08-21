package com.piano.server.domain.DTO;

import io.micrometer.common.lang.NonNull;
import lombok.Data;

@Data
public class DeleteSongRequest {
    @NonNull
    Long id;
}
