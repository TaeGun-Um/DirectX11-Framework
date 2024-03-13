struct ParticleData
{
    // 현재 위치
    float4 vRelativePos;
    // 파티클의 방향
    float4 vDir;
		
    // 시간 사라지는시간
    float fMaxTime;
    // 현재 시간
    float fCurTime;
    // 이동할때의 스피드
    float fSpeed;
    // 살아있냐 죽어있냐.
    uint iActive;
};
