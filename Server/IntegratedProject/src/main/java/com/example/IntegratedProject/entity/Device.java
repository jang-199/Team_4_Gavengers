package com.example.IntegratedProject.entity;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;
import lombok.ToString;

import javax.persistence.*;

@Data
@NoArgsConstructor // 파라미터가 없는 생성자를 생성
@AllArgsConstructor // 클래스에 존재하는 모든 필드에 대한 생성자를 자동으로 생성.
@ToString
@Entity
public class Device {
    @Id // Primary Key 지정
    @Column(name = "DEVICE_ID") // 컬럼 지정
    private String id;
}
