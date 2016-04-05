	subroutine test( M, N, A, LDA, IPIV, INFO, RANK, BS)
	INTEGER INFO, LDA, M, N, RANK, BS
	INTEGER IPIV( * )
	DOUBLE PRECISION A( LDA, * )

	DOUBLE PRECISION ONE
	PARAMETER ( ONE = 1.0D+0 )

	INTEGER I, IINFO, J, JB, NB
	EXTERNAL DGEMM, DGETF2, DLASWP, DTRSM, XERBLA
	INTEGER ILAENV
	EXTERNAL ILAENV
	INTRINSIC MAX, MIN

	DOUBLE PRECISION t_time
	DOUBLE PRECISION t_start
	DOUBLE PRECISION t_stop
	t_time = 0
	CALL cpu_time( t_start );

	INFO = 0
	IF( M.LT.0 ) THEN
		INFO = -1
	ELSE IF( N.LT.0 ) THEN
		INFO = -2
	ELSE IF( RANK.GT.MIN( M, N ) ) THEN
		INFO = -7
	END IF
	IF ( INFO.NE.0 ) THEN
		CALL XERBLA( 'DGETRF', -INFO )
		RETURN
	END IF

	IF( M.EQ.0 .OR. N.EQ.0 ) THEN
		RETURN
	END IF

	NB = ILAENV( 1, 'DGETRF', ' ', M, N, -1, -1 )
C	print *, NB

	NB = BS

	IF( NB.LE.1 .OR. NB.GE.MIN( M, N ) ) THEN
		CALL DGETF2( M, N, A, LDA, IPIV, INFO )
	ELSE
		DO J = 1, RANK, NB
			JB = MIN( RANK-J+1, NB )
			CALL DGETF2( M-J+1, JB, A( J, J ), LDA, IPIV( J ), IINFO )
			IF( INFO.EQ.0 .AND. IINFO.GT.0 ) THEN
				INFO = IINFO + J - 1
			END IF
			DO I = J, MIN( M, J+JB-1 )
				IPIV( I ) = J - 1 + IPIV( I )
			END DO
			CALL DLASWP( N-J-JB+1, A( 1, J+JB ), LDA, J, J+JB-1, IPIV, 1 )
			integer t = N-J-JB+1
			CALL DTRSM('L','L','N','U',JB,N-J-JB+1,ONE,
     $ 				A(J,J),LDA,A(J,J+JB),LDA)
			IF( J+JB.LE.M ) THEN
C				print *, LDA
C				CALL cpu_time(t_start)
				CALL DGEMM('N','N',M-J-JB+1,N-J-JB+1,JB,-ONE,A(J+JB,J),
     $ 						LDA,A(J,J+JB),LDA,ONE,
     $ 						A(J+JB,J+JB),LDA)
C				CALL cpu_time( t_stop )
C				t_time = t_time + t_stop - t_start
C				print *, t_time
C				print *, t_start
C				print *, t_stop
			END IF
		END DO
	END IF

	CALL cpu_time( t_stop )
	t_time = t_time + t_stop - t_start
C	print *, t_time
	return
	end
