package com.piano.server.domain.DTO;

import io.micrometer.common.lang.NonNull;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@NoArgsConstructor
@AllArgsConstructor
public class GetSongResponse {
    @NonNull
    private Long id;
    @NonNull
    private String title;
    @NonNull
    private String notes;
}
