/*---------------------------------------------------------------------------*\
| =========                 |                                                 |
| \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox           |
|  \\    /   O peration     | Version:  v1906                                 |
|   \\  /    A nd           | Web:      www.OpenFOAM.com                      |
|    \\/     M anipulation  |                                                 |
\*---------------------------------------------------------------------------*/
Build  : v1906 OPENFOAM=1906
Arch   : "LSB;label=32;scalar=64"
Exec   : dualZoneEulerFoam
Date   : Jan 03 2021
Time   : 12:00:52
Host   : usv001
PID    : 16540
I/O    : uncollated
Case   : /home/usv001/OpenFOAM/usv001-v1906/run/Acoustics/case03_singleGaussianWave
nProcs : 1
trapFpe: Floating point exception trapping enabled (FOAM_SIGFPE).
fileModificationChecking : Monitoring run-time modified files using timeStampMaster (fileModificationSkew 10)
allowSystemOperations : Allowing user-supplied system call operations

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
Create time

Create dual region fvMesh for time = 0

Spatial scheme: DRP7

Filter scheme: SFo9p

Creating mesh-to-mesh addressing for linearZone and nonLinearZone regions using cellVolumeWeight
    Overlap volume: 49.75

Temporal scheme: LDDRK46

Reading thermophysical properties

Using #calc at line 32 in file "/home/usv001/OpenFOAM/usv001-v1906/run/Acoustics/case03_singleGaussianWave/constant/linearZone/thermophysicalProperties.mixture"
Using #codeStream with "/home/usv001/OpenFOAM/usv001-v1906/run/Acoustics/case03_singleGaussianWave/dynamicCode/platforms/linux64GccDPInt32Opt/lib/libcodeStream_72bb7d33e38fb5c92a2a7201104084fc5ac2f828.so"
Using #calc at line 36 in file "/home/usv001/OpenFOAM/usv001-v1906/run/Acoustics/case03_singleGaussianWave/constant/linearZone/thermophysicalProperties.mixture.specie"
Using #codeStream with "/home/usv001/OpenFOAM/usv001-v1906/run/Acoustics/case03_singleGaussianWave/dynamicCode/platforms/linux64GccDPInt32Opt/lib/libcodeStream_b5064eab5ba53e3ee92624b7822f4735cb3d1bcc.so"
Using #calc at line 40 in file "/home/usv001/OpenFOAM/usv001-v1906/run/Acoustics/case03_singleGaussianWave/constant/linearZone/thermophysicalProperties.mixture.thermodynamics"
Using #codeStream with "/home/usv001/OpenFOAM/usv001-v1906/run/Acoustics/case03_singleGaussianWave/dynamicCode/platforms/linux64GccDPInt32Opt/lib/libcodeStream_17009833044708a64da23f4e6cd5e1b073490001.so"
Selecting thermodynamics package 
{
    type            hePsiThermo;
    mixture         pureMixture;
    transport       const;
    thermo          hConst;
    equationOfState perfectGas;
    specie          specie;
    energy          sensibleInternalEnergy;
}

Using #calc at line 32 in file "/home/usv001/OpenFOAM/usv001-v1906/run/Acoustics/case03_singleGaussianWave/constant/linearZone/thermophysicalProperties.mixture"
Using #calc at line 36 in file "/home/usv001/OpenFOAM/usv001-v1906/run/Acoustics/case03_singleGaussianWave/constant/linearZone/thermophysicalProperties.mixture.specie"
Using #calc at line 40 in file "/home/usv001/OpenFOAM/usv001-v1906/run/Acoustics/case03_singleGaussianWave/constant/linearZone/thermophysicalProperties.mixture.thermodynamics"
Using #calc at line 32 in file "/home/usv001/OpenFOAM/usv001-v1906/run/Acoustics/case03_singleGaussianWave/constant/nonLinearZone/thermophysicalProperties.mixture"
Using #calc at line 36 in file "/home/usv001/OpenFOAM/usv001-v1906/run/Acoustics/case03_singleGaussianWave/constant/nonLinearZone/thermophysicalProperties.mixture.specie"
Using #calc at line 40 in file "/home/usv001/OpenFOAM/usv001-v1906/run/Acoustics/case03_singleGaussianWave/constant/nonLinearZone/thermophysicalProperties.mixture.thermodynamics"
Selecting thermodynamics package 
{
    type            hePsiThermo;
    mixture         pureMixture;
    transport       const;
    thermo          hConst;
    equationOfState perfectGas;
    specie          specie;
    energy          sensibleInternalEnergy;
}

Using #calc at line 32 in file "/home/usv001/OpenFOAM/usv001-v1906/run/Acoustics/case03_singleGaussianWave/constant/nonLinearZone/thermophysicalProperties.mixture"
Using #calc at line 36 in file "/home/usv001/OpenFOAM/usv001-v1906/run/Acoustics/case03_singleGaussianWave/constant/nonLinearZone/thermophysicalProperties.mixture.specie"
Using #calc at line 40 in file "/home/usv001/OpenFOAM/usv001-v1906/run/Acoustics/case03_singleGaussianWave/constant/nonLinearZone/thermophysicalProperties.mixture.thermodynamics"
Using #codeStream at line 22 in file "/home/usv001/OpenFOAM/usv001-v1906/run/Acoustics/case03_singleGaussianWave/0/nonLinearZone/p"
Using #codeStream with "/home/usv001/OpenFOAM/usv001-v1906/run/Acoustics/case03_singleGaussianWave/dynamicCode/platforms/linux64GccDPInt32Opt/lib/libcodeStream_4ad67921841ebc2a0ac0654b1576cc0cbf2144ab.so"
Using #codeStream at line 22 in file "/home/usv001/OpenFOAM/usv001-v1906/run/Acoustics/case03_singleGaussianWave/0/nonLinearZone/T"
Using #codeStream with "/home/usv001/OpenFOAM/usv001-v1906/run/Acoustics/case03_singleGaussianWave/dynamicCode/platforms/linux64GccDPInt32Opt/lib/libcodeStream_9f1ba5703f49a01ada63e3011936bc1851a2ee24.so"
Using #codeStream at line 22 in file "/home/usv001/OpenFOAM/usv001-v1906/run/Acoustics/case03_singleGaussianWave/0/nonLinearZone/U"
Using #codeStream with "/home/usv001/OpenFOAM/usv001-v1906/run/Acoustics/case03_singleGaussianWave/dynamicCode/platforms/linux64GccDPInt32Opt/lib/libcodeStream_9c42a143145efb694fd4ff90ba7d0b371e7fb970.so"
Using #codeStream at line 22 in file "/home/usv001/OpenFOAM/usv001-v1906/run/Acoustics/case03_singleGaussianWave/0/linearZone/rhoPrime"
Using #codeStream with "/home/usv001/OpenFOAM/usv001-v1906/run/Acoustics/case03_singleGaussianWave/dynamicCode/platforms/linux64GccDPInt32Opt/lib/libcodeStream_144fa83afad973a3bda0fe747187380fa3ca0487.so"
Using #codeStream at line 22 in file "/home/usv001/OpenFOAM/usv001-v1906/run/Acoustics/case03_singleGaussianWave/0/linearZone/UPrime"
Using #codeStream at line 22 in file "/home/usv001/OpenFOAM/usv001-v1906/run/Acoustics/case03_singleGaussianWave/0/linearZone/pPrime"
Using #codeStream with "/home/usv001/OpenFOAM/usv001-v1906/run/Acoustics/case03_singleGaussianWave/dynamicCode/platforms/linux64GccDPInt32Opt/lib/libcodeStream_5016aff08910e4a4d100623941e3d6ba79173a9c.so"

Reading spongeZonesDict

No finite volume options present

Starting time loop

Using dynamicCode for functionObject energyConservationError at line -1 in "/home/usv001/OpenFOAM/usv001-v1906/run/Acoustics/case03_singleGaussianWave/system/controlDict.functions.energyConservationError"
RunTime = 0.125
ExecutionTime = 0.05 s  ClockTime = 0 s

RunTime = 0.25
ExecutionTime = 0.06 s  ClockTime = 0 s

RunTime = 0.375
ExecutionTime = 0.06 s  ClockTime = 0 s

RunTime = 0.5
ExecutionTime = 0.06 s  ClockTime = 0 s

RunTime = 0.625
ExecutionTime = 0.06 s  ClockTime = 0 s

RunTime = 0.75
ExecutionTime = 0.07 s  ClockTime = 0 s

RunTime = 0.875
ExecutionTime = 0.07 s  ClockTime = 0 s

RunTime = 1
ExecutionTime = 0.07 s  ClockTime = 0 s

RunTime = 1.125
ExecutionTime = 0.08 s  ClockTime = 0 s

RunTime = 1.25
ExecutionTime = 0.08 s  ClockTime = 0 s

RunTime = 1.375
ExecutionTime = 0.08 s  ClockTime = 0 s

RunTime = 1.5
ExecutionTime = 0.08 s  ClockTime = 0 s

RunTime = 1.625
ExecutionTime = 0.09 s  ClockTime = 0 s

RunTime = 1.75
ExecutionTime = 0.09 s  ClockTime = 0 s

RunTime = 1.875
ExecutionTime = 0.09 s  ClockTime = 0 s

RunTime = 2
ExecutionTime = 0.09 s  ClockTime = 0 s

RunTime = 2.125
ExecutionTime = 0.1 s  ClockTime = 0 s

RunTime = 2.25
ExecutionTime = 0.1 s  ClockTime = 0 s

RunTime = 2.375
ExecutionTime = 0.1 s  ClockTime = 0 s

RunTime = 2.5
ExecutionTime = 0.11 s  ClockTime = 0 s

RunTime = 2.625
ExecutionTime = 0.11 s  ClockTime = 0 s

RunTime = 2.75
ExecutionTime = 0.11 s  ClockTime = 0 s

RunTime = 2.875
ExecutionTime = 0.12 s  ClockTime = 0 s

RunTime = 3
ExecutionTime = 0.12 s  ClockTime = 0 s

RunTime = 3.125
ExecutionTime = 0.12 s  ClockTime = 0 s

RunTime = 3.25
ExecutionTime = 0.12 s  ClockTime = 0 s

RunTime = 3.375
ExecutionTime = 0.13 s  ClockTime = 0 s

RunTime = 3.5
ExecutionTime = 0.13 s  ClockTime = 0 s

RunTime = 3.625
ExecutionTime = 0.13 s  ClockTime = 0 s

RunTime = 3.75
ExecutionTime = 0.13 s  ClockTime = 0 s

RunTime = 3.875
ExecutionTime = 0.14 s  ClockTime = 0 s

RunTime = 4
ExecutionTime = 0.14 s  ClockTime = 0 s

RunTime = 4.125
ExecutionTime = 0.14 s  ClockTime = 0 s

RunTime = 4.25
ExecutionTime = 0.15 s  ClockTime = 0 s

RunTime = 4.375
ExecutionTime = 0.15 s  ClockTime = 0 s

RunTime = 4.5
ExecutionTime = 0.15 s  ClockTime = 0 s

RunTime = 4.625
ExecutionTime = 0.15 s  ClockTime = 0 s

RunTime = 4.75
ExecutionTime = 0.16 s  ClockTime = 0 s

RunTime = 4.875
ExecutionTime = 0.16 s  ClockTime = 0 s

RunTime = 5
ExecutionTime = 0.16 s  ClockTime = 0 s

RunTime = 5.125
ExecutionTime = 0.16 s  ClockTime = 0 s

RunTime = 5.25
ExecutionTime = 0.17 s  ClockTime = 0 s

RunTime = 5.375
ExecutionTime = 0.17 s  ClockTime = 0 s

RunTime = 5.5
ExecutionTime = 0.17 s  ClockTime = 0 s

RunTime = 5.625
ExecutionTime = 0.18 s  ClockTime = 0 s

RunTime = 5.75
ExecutionTime = 0.18 s  ClockTime = 0 s

RunTime = 5.875
ExecutionTime = 0.18 s  ClockTime = 0 s

RunTime = 6
ExecutionTime = 0.18 s  ClockTime = 0 s

RunTime = 6.125
ExecutionTime = 0.19 s  ClockTime = 0 s

RunTime = 6.25
ExecutionTime = 0.19 s  ClockTime = 0 s

RunTime = 6.375
ExecutionTime = 0.19 s  ClockTime = 0 s

RunTime = 6.5
ExecutionTime = 0.19 s  ClockTime = 0 s

RunTime = 6.625
ExecutionTime = 0.2 s  ClockTime = 0 s

RunTime = 6.75
ExecutionTime = 0.2 s  ClockTime = 0 s

RunTime = 6.875
ExecutionTime = 0.2 s  ClockTime = 0 s

RunTime = 7
ExecutionTime = 0.2 s  ClockTime = 0 s

RunTime = 7.125
ExecutionTime = 0.21 s  ClockTime = 0 s

RunTime = 7.25
ExecutionTime = 0.21 s  ClockTime = 0 s

RunTime = 7.375
ExecutionTime = 0.21 s  ClockTime = 0 s

RunTime = 7.5
ExecutionTime = 0.22 s  ClockTime = 0 s

RunTime = 7.625
ExecutionTime = 0.22 s  ClockTime = 0 s

RunTime = 7.75
ExecutionTime = 0.22 s  ClockTime = 0 s

RunTime = 7.875
ExecutionTime = 0.22 s  ClockTime = 0 s

RunTime = 8
ExecutionTime = 0.23 s  ClockTime = 0 s

RunTime = 8.125
ExecutionTime = 0.23 s  ClockTime = 0 s

RunTime = 8.25
ExecutionTime = 0.23 s  ClockTime = 0 s

RunTime = 8.375
ExecutionTime = 0.23 s  ClockTime = 0 s

RunTime = 8.5
ExecutionTime = 0.24 s  ClockTime = 0 s

RunTime = 8.625
ExecutionTime = 0.24 s  ClockTime = 0 s

RunTime = 8.75
ExecutionTime = 0.24 s  ClockTime = 0 s

RunTime = 8.875
ExecutionTime = 0.24 s  ClockTime = 0 s

RunTime = 9
ExecutionTime = 0.25 s  ClockTime = 0 s

RunTime = 9.125
ExecutionTime = 0.25 s  ClockTime = 0 s

RunTime = 9.25
ExecutionTime = 0.25 s  ClockTime = 0 s

RunTime = 9.375
ExecutionTime = 0.26 s  ClockTime = 0 s

RunTime = 9.5
ExecutionTime = 0.26 s  ClockTime = 0 s

RunTime = 9.625
ExecutionTime = 0.26 s  ClockTime = 0 s

RunTime = 9.75
ExecutionTime = 0.26 s  ClockTime = 0 s

RunTime = 9.875
ExecutionTime = 0.27 s  ClockTime = 0 s

RunTime = 10
ExecutionTime = 0.27 s  ClockTime = 0 s

RunTime = 10.125
ExecutionTime = 0.27 s  ClockTime = 0 s

RunTime = 10.25
ExecutionTime = 0.27 s  ClockTime = 0 s

RunTime = 10.375
ExecutionTime = 0.28 s  ClockTime = 0 s

RunTime = 10.5
ExecutionTime = 0.28 s  ClockTime = 0 s

RunTime = 10.625
ExecutionTime = 0.28 s  ClockTime = 0 s

RunTime = 10.75
ExecutionTime = 0.29 s  ClockTime = 0 s

RunTime = 10.875
ExecutionTime = 0.29 s  ClockTime = 0 s

RunTime = 11
ExecutionTime = 0.29 s  ClockTime = 0 s

RunTime = 11.125
ExecutionTime = 0.29 s  ClockTime = 0 s

RunTime = 11.25
ExecutionTime = 0.3 s  ClockTime = 0 s

RunTime = 11.375
ExecutionTime = 0.3 s  ClockTime = 0 s

RunTime = 11.5
ExecutionTime = 0.3 s  ClockTime = 0 s

RunTime = 11.625
ExecutionTime = 0.3 s  ClockTime = 0 s

RunTime = 11.75
ExecutionTime = 0.31 s  ClockTime = 0 s

RunTime = 11.875
ExecutionTime = 0.31 s  ClockTime = 0 s

RunTime = 12
ExecutionTime = 0.31 s  ClockTime = 0 s

RunTime = 12.125
ExecutionTime = 0.32 s  ClockTime = 0 s

RunTime = 12.25
ExecutionTime = 0.32 s  ClockTime = 0 s

RunTime = 12.375
ExecutionTime = 0.32 s  ClockTime = 0 s

RunTime = 12.5
ExecutionTime = 0.32 s  ClockTime = 0 s

RunTime = 12.625
ExecutionTime = 0.33 s  ClockTime = 0 s

RunTime = 12.75
ExecutionTime = 0.33 s  ClockTime = 0 s

RunTime = 12.875
ExecutionTime = 0.33 s  ClockTime = 0 s

RunTime = 13
ExecutionTime = 0.33 s  ClockTime = 0 s

RunTime = 13.125
ExecutionTime = 0.34 s  ClockTime = 0 s

RunTime = 13.25
ExecutionTime = 0.34 s  ClockTime = 0 s

RunTime = 13.375
ExecutionTime = 0.34 s  ClockTime = 0 s

RunTime = 13.5
ExecutionTime = 0.35 s  ClockTime = 0 s

RunTime = 13.625
ExecutionTime = 0.35 s  ClockTime = 0 s

RunTime = 13.75
ExecutionTime = 0.35 s  ClockTime = 0 s

RunTime = 13.875
ExecutionTime = 0.35 s  ClockTime = 0 s

RunTime = 14
ExecutionTime = 0.36 s  ClockTime = 0 s

RunTime = 14.125
ExecutionTime = 0.36 s  ClockTime = 0 s

RunTime = 14.25
ExecutionTime = 0.36 s  ClockTime = 0 s

RunTime = 14.375
ExecutionTime = 0.37 s  ClockTime = 0 s

RunTime = 14.5
ExecutionTime = 0.37 s  ClockTime = 0 s

RunTime = 14.625
ExecutionTime = 0.37 s  ClockTime = 0 s

RunTime = 14.75
ExecutionTime = 0.37 s  ClockTime = 0 s

RunTime = 14.875
ExecutionTime = 0.38 s  ClockTime = 0 s

RunTime = 15
ExecutionTime = 0.38 s  ClockTime = 0 s

RunTime = 15.125
ExecutionTime = 0.38 s  ClockTime = 0 s

RunTime = 15.25
ExecutionTime = 0.38 s  ClockTime = 0 s

RunTime = 15.375
ExecutionTime = 0.39 s  ClockTime = 0 s

RunTime = 15.5
ExecutionTime = 0.39 s  ClockTime = 0 s

RunTime = 15.625
ExecutionTime = 0.39 s  ClockTime = 0 s

RunTime = 15.75
ExecutionTime = 0.4 s  ClockTime = 0 s

RunTime = 15.875
ExecutionTime = 0.4 s  ClockTime = 0 s

RunTime = 16
ExecutionTime = 0.4 s  ClockTime = 0 s

RunTime = 16.125
ExecutionTime = 0.4 s  ClockTime = 0 s

RunTime = 16.25
ExecutionTime = 0.41 s  ClockTime = 0 s

RunTime = 16.375
ExecutionTime = 0.41 s  ClockTime = 0 s

RunTime = 16.5
ExecutionTime = 0.41 s  ClockTime = 0 s

RunTime = 16.625
ExecutionTime = 0.41 s  ClockTime = 0 s

RunTime = 16.75
ExecutionTime = 0.42 s  ClockTime = 0 s

RunTime = 16.875
ExecutionTime = 0.42 s  ClockTime = 0 s

RunTime = 17
ExecutionTime = 0.42 s  ClockTime = 0 s

RunTime = 17.125
ExecutionTime = 0.43 s  ClockTime = 0 s

RunTime = 17.25
ExecutionTime = 0.43 s  ClockTime = 0 s

RunTime = 17.375
ExecutionTime = 0.43 s  ClockTime = 0 s

RunTime = 17.5
ExecutionTime = 0.43 s  ClockTime = 0 s

RunTime = 17.625
ExecutionTime = 0.44 s  ClockTime = 0 s

RunTime = 17.75
ExecutionTime = 0.44 s  ClockTime = 0 s

RunTime = 17.875
ExecutionTime = 0.44 s  ClockTime = 0 s

RunTime = 18
ExecutionTime = 0.45 s  ClockTime = 0 s

RunTime = 18.125
ExecutionTime = 0.45 s  ClockTime = 0 s

RunTime = 18.25
ExecutionTime = 0.45 s  ClockTime = 0 s

RunTime = 18.375
ExecutionTime = 0.46 s  ClockTime = 0 s

RunTime = 18.5
ExecutionTime = 0.46 s  ClockTime = 0 s

RunTime = 18.625
ExecutionTime = 0.46 s  ClockTime = 0 s

RunTime = 18.75
ExecutionTime = 0.46 s  ClockTime = 0 s

RunTime = 18.875
ExecutionTime = 0.47 s  ClockTime = 0 s

RunTime = 19
ExecutionTime = 0.47 s  ClockTime = 0 s

RunTime = 19.125
ExecutionTime = 0.47 s  ClockTime = 0 s

RunTime = 19.25
ExecutionTime = 0.47 s  ClockTime = 0 s

RunTime = 19.375
ExecutionTime = 0.48 s  ClockTime = 0 s

RunTime = 19.5
ExecutionTime = 0.48 s  ClockTime = 0 s

RunTime = 19.625
ExecutionTime = 0.48 s  ClockTime = 0 s

RunTime = 19.75
ExecutionTime = 0.49 s  ClockTime = 0 s

RunTime = 19.875
ExecutionTime = 0.49 s  ClockTime = 0 s

RunTime = 20
ExecutionTime = 0.49 s  ClockTime = 0 s

RunTime = 20.125
ExecutionTime = 0.49 s  ClockTime = 0 s

RunTime = 20.25
ExecutionTime = 0.5 s  ClockTime = 0 s

RunTime = 20.375
ExecutionTime = 0.5 s  ClockTime = 0 s

RunTime = 20.5
ExecutionTime = 0.5 s  ClockTime = 0 s

RunTime = 20.625
ExecutionTime = 0.5 s  ClockTime = 0 s

RunTime = 20.75
ExecutionTime = 0.51 s  ClockTime = 0 s

RunTime = 20.875
ExecutionTime = 0.51 s  ClockTime = 0 s

RunTime = 21
ExecutionTime = 0.51 s  ClockTime = 0 s

RunTime = 21.125
ExecutionTime = 0.52 s  ClockTime = 0 s

RunTime = 21.25
ExecutionTime = 0.52 s  ClockTime = 0 s

RunTime = 21.375
ExecutionTime = 0.52 s  ClockTime = 0 s

RunTime = 21.5
ExecutionTime = 0.53 s  ClockTime = 0 s

RunTime = 21.625
ExecutionTime = 0.53 s  ClockTime = 0 s

RunTime = 21.75
ExecutionTime = 0.53 s  ClockTime = 0 s

RunTime = 21.875
ExecutionTime = 0.53 s  ClockTime = 0 s

RunTime = 22
ExecutionTime = 0.54 s  ClockTime = 0 s

RunTime = 22.125
ExecutionTime = 0.54 s  ClockTime = 0 s

RunTime = 22.25
ExecutionTime = 0.54 s  ClockTime = 0 s

RunTime = 22.375
ExecutionTime = 0.54 s  ClockTime = 0 s

RunTime = 22.5
ExecutionTime = 0.55 s  ClockTime = 0 s

RunTime = 22.625
ExecutionTime = 0.55 s  ClockTime = 0 s

RunTime = 22.75
ExecutionTime = 0.55 s  ClockTime = 0 s

RunTime = 22.875
ExecutionTime = 0.56 s  ClockTime = 0 s

RunTime = 23
ExecutionTime = 0.56 s  ClockTime = 0 s

RunTime = 23.125
ExecutionTime = 0.56 s  ClockTime = 0 s

RunTime = 23.25
ExecutionTime = 0.56 s  ClockTime = 0 s

RunTime = 23.375
ExecutionTime = 0.57 s  ClockTime = 0 s

RunTime = 23.5
ExecutionTime = 0.57 s  ClockTime = 0 s

RunTime = 23.625
ExecutionTime = 0.57 s  ClockTime = 0 s

RunTime = 23.75
ExecutionTime = 0.57 s  ClockTime = 0 s

RunTime = 23.875
ExecutionTime = 0.58 s  ClockTime = 0 s

RunTime = 24
ExecutionTime = 0.58 s  ClockTime = 0 s

RunTime = 24.125
ExecutionTime = 0.58 s  ClockTime = 0 s

RunTime = 24.25
ExecutionTime = 0.59 s  ClockTime = 0 s

RunTime = 24.375
ExecutionTime = 0.59 s  ClockTime = 0 s

RunTime = 24.5
ExecutionTime = 0.59 s  ClockTime = 0 s

RunTime = 24.625
ExecutionTime = 0.59 s  ClockTime = 0 s

RunTime = 24.75
ExecutionTime = 0.6 s  ClockTime = 0 s

RunTime = 24.875
ExecutionTime = 0.6 s  ClockTime = 0 s

RunTime = 25
ExecutionTime = 0.6 s  ClockTime = 0 s

RunTime = 25.125
ExecutionTime = 0.6 s  ClockTime = 0 s

RunTime = 25.25
ExecutionTime = 0.61 s  ClockTime = 0 s

RunTime = 25.375
ExecutionTime = 0.61 s  ClockTime = 0 s

RunTime = 25.5
ExecutionTime = 0.61 s  ClockTime = 0 s

RunTime = 25.625
ExecutionTime = 0.62 s  ClockTime = 0 s

RunTime = 25.75
ExecutionTime = 0.62 s  ClockTime = 0 s

RunTime = 25.875
ExecutionTime = 0.62 s  ClockTime = 0 s

RunTime = 26
ExecutionTime = 0.62 s  ClockTime = 0 s

RunTime = 26.125
ExecutionTime = 0.63 s  ClockTime = 0 s

RunTime = 26.25
ExecutionTime = 0.63 s  ClockTime = 0 s

RunTime = 26.375
ExecutionTime = 0.63 s  ClockTime = 0 s

RunTime = 26.5
ExecutionTime = 0.64 s  ClockTime = 0 s

RunTime = 26.625
ExecutionTime = 0.64 s  ClockTime = 0 s

RunTime = 26.75
ExecutionTime = 0.64 s  ClockTime = 0 s

RunTime = 26.875
ExecutionTime = 0.64 s  ClockTime = 0 s

RunTime = 27
ExecutionTime = 0.65 s  ClockTime = 0 s

RunTime = 27.125
ExecutionTime = 0.65 s  ClockTime = 0 s

RunTime = 27.25
ExecutionTime = 0.65 s  ClockTime = 0 s

RunTime = 27.375
ExecutionTime = 0.65 s  ClockTime = 0 s

RunTime = 27.5
ExecutionTime = 0.66 s  ClockTime = 0 s

RunTime = 27.625
ExecutionTime = 0.66 s  ClockTime = 0 s

RunTime = 27.75
ExecutionTime = 0.66 s  ClockTime = 0 s

RunTime = 27.875
ExecutionTime = 0.67 s  ClockTime = 0 s

RunTime = 28
ExecutionTime = 0.67 s  ClockTime = 0 s

RunTime = 28.125
ExecutionTime = 0.67 s  ClockTime = 0 s

RunTime = 28.25
ExecutionTime = 0.67 s  ClockTime = 0 s

RunTime = 28.375
ExecutionTime = 0.68 s  ClockTime = 0 s

RunTime = 28.5
ExecutionTime = 0.68 s  ClockTime = 0 s

RunTime = 28.625
ExecutionTime = 0.68 s  ClockTime = 0 s

RunTime = 28.75
ExecutionTime = 0.68 s  ClockTime = 0 s

RunTime = 28.875
ExecutionTime = 0.69 s  ClockTime = 0 s

RunTime = 29
ExecutionTime = 0.69 s  ClockTime = 0 s

RunTime = 29.125
ExecutionTime = 0.69 s  ClockTime = 0 s

RunTime = 29.25
ExecutionTime = 0.7 s  ClockTime = 0 s

RunTime = 29.375
ExecutionTime = 0.7 s  ClockTime = 0 s

RunTime = 29.5
ExecutionTime = 0.7 s  ClockTime = 0 s

RunTime = 29.625
ExecutionTime = 0.7 s  ClockTime = 0 s

RunTime = 29.75
ExecutionTime = 0.71 s  ClockTime = 0 s

RunTime = 29.875
ExecutionTime = 0.71 s  ClockTime = 0 s

RunTime = 30
ExecutionTime = 0.71 s  ClockTime = 0 s

RunTime = 30.125
ExecutionTime = 0.71 s  ClockTime = 0 s

RunTime = 30.25
ExecutionTime = 0.72 s  ClockTime = 0 s

RunTime = 30.375
ExecutionTime = 0.72 s  ClockTime = 0 s

RunTime = 30.5
ExecutionTime = 0.72 s  ClockTime = 0 s

RunTime = 30.625
ExecutionTime = 0.73 s  ClockTime = 0 s

RunTime = 30.75
ExecutionTime = 0.73 s  ClockTime = 0 s

RunTime = 30.875
ExecutionTime = 0.73 s  ClockTime = 0 s

RunTime = 31
ExecutionTime = 0.73 s  ClockTime = 0 s

RunTime = 31.125
ExecutionTime = 0.74 s  ClockTime = 0 s

RunTime = 31.25
ExecutionTime = 0.74 s  ClockTime = 0 s

RunTime = 31.375
ExecutionTime = 0.74 s  ClockTime = 0 s

RunTime = 31.5
ExecutionTime = 0.74 s  ClockTime = 0 s

RunTime = 31.625
ExecutionTime = 0.75 s  ClockTime = 0 s

RunTime = 31.75
ExecutionTime = 0.75 s  ClockTime = 0 s

RunTime = 31.875
ExecutionTime = 0.75 s  ClockTime = 0 s

RunTime = 32
ExecutionTime = 0.76 s  ClockTime = 0 s

RunTime = 32.125
ExecutionTime = 0.76 s  ClockTime = 0 s

RunTime = 32.25
ExecutionTime = 0.76 s  ClockTime = 0 s

RunTime = 32.375
ExecutionTime = 0.76 s  ClockTime = 0 s

RunTime = 32.5
ExecutionTime = 0.77 s  ClockTime = 0 s

RunTime = 32.625
ExecutionTime = 0.77 s  ClockTime = 0 s

RunTime = 32.75
ExecutionTime = 0.77 s  ClockTime = 0 s

RunTime = 32.875
ExecutionTime = 0.77 s  ClockTime = 0 s

RunTime = 33
ExecutionTime = 0.78 s  ClockTime = 0 s

RunTime = 33.125
ExecutionTime = 0.78 s  ClockTime = 0 s

RunTime = 33.25
ExecutionTime = 0.78 s  ClockTime = 0 s

RunTime = 33.375
ExecutionTime = 0.79 s  ClockTime = 0 s

RunTime = 33.5
ExecutionTime = 0.79 s  ClockTime = 0 s

RunTime = 33.625
ExecutionTime = 0.79 s  ClockTime = 0 s

RunTime = 33.75
ExecutionTime = 0.79 s  ClockTime = 0 s

RunTime = 33.875
ExecutionTime = 0.8 s  ClockTime = 0 s

RunTime = 34
ExecutionTime = 0.8 s  ClockTime = 0 s

RunTime = 34.125
ExecutionTime = 0.8 s  ClockTime = 0 s

RunTime = 34.25
ExecutionTime = 0.8 s  ClockTime = 0 s

RunTime = 34.375
ExecutionTime = 0.81 s  ClockTime = 0 s

RunTime = 34.5
ExecutionTime = 0.81 s  ClockTime = 0 s

RunTime = 34.625
ExecutionTime = 0.81 s  ClockTime = 0 s

RunTime = 34.75
ExecutionTime = 0.82 s  ClockTime = 0 s

RunTime = 34.875
ExecutionTime = 0.82 s  ClockTime = 0 s

RunTime = 35
ExecutionTime = 0.82 s  ClockTime = 0 s

RunTime = 35.125
ExecutionTime = 0.82 s  ClockTime = 0 s

RunTime = 35.25
ExecutionTime = 0.83 s  ClockTime = 0 s

RunTime = 35.375
ExecutionTime = 0.83 s  ClockTime = 0 s

RunTime = 35.5
ExecutionTime = 0.83 s  ClockTime = 0 s

RunTime = 35.625
ExecutionTime = 0.83 s  ClockTime = 0 s

RunTime = 35.75
ExecutionTime = 0.84 s  ClockTime = 0 s

RunTime = 35.875
ExecutionTime = 0.84 s  ClockTime = 0 s

RunTime = 36
ExecutionTime = 0.84 s  ClockTime = 0 s

RunTime = 36.125
ExecutionTime = 0.85 s  ClockTime = 0 s

RunTime = 36.25
ExecutionTime = 0.85 s  ClockTime = 0 s

RunTime = 36.375
ExecutionTime = 0.85 s  ClockTime = 0 s

RunTime = 36.5
ExecutionTime = 0.86 s  ClockTime = 0 s

RunTime = 36.625
ExecutionTime = 0.86 s  ClockTime = 0 s

RunTime = 36.75
ExecutionTime = 0.86 s  ClockTime = 0 s

RunTime = 36.875
ExecutionTime = 0.86 s  ClockTime = 0 s

RunTime = 37
ExecutionTime = 0.87 s  ClockTime = 0 s

RunTime = 37.125
ExecutionTime = 0.87 s  ClockTime = 0 s

RunTime = 37.25
ExecutionTime = 0.87 s  ClockTime = 0 s

RunTime = 37.375
ExecutionTime = 0.87 s  ClockTime = 0 s

RunTime = 37.5
ExecutionTime = 0.88 s  ClockTime = 0 s

RunTime = 37.625
ExecutionTime = 0.88 s  ClockTime = 0 s

RunTime = 37.75
ExecutionTime = 0.88 s  ClockTime = 0 s

RunTime = 37.875
ExecutionTime = 0.89 s  ClockTime = 0 s

RunTime = 38
ExecutionTime = 0.89 s  ClockTime = 0 s

RunTime = 38.125
ExecutionTime = 0.89 s  ClockTime = 0 s

RunTime = 38.25
ExecutionTime = 0.89 s  ClockTime = 0 s

RunTime = 38.375
ExecutionTime = 0.9 s  ClockTime = 0 s

RunTime = 38.5
ExecutionTime = 0.9 s  ClockTime = 0 s

RunTime = 38.625
ExecutionTime = 0.9 s  ClockTime = 0 s

RunTime = 38.75
ExecutionTime = 0.9 s  ClockTime = 0 s

RunTime = 38.875
ExecutionTime = 0.91 s  ClockTime = 0 s

RunTime = 39
ExecutionTime = 0.91 s  ClockTime = 0 s

RunTime = 39.125
ExecutionTime = 0.91 s  ClockTime = 0 s

RunTime = 39.25
ExecutionTime = 0.92 s  ClockTime = 0 s

RunTime = 39.375
ExecutionTime = 0.92 s  ClockTime = 0 s

RunTime = 39.5
ExecutionTime = 0.92 s  ClockTime = 1 s

RunTime = 39.625
ExecutionTime = 0.92 s  ClockTime = 1 s

RunTime = 39.75
ExecutionTime = 0.93 s  ClockTime = 1 s

RunTime = 39.875
ExecutionTime = 0.93 s  ClockTime = 1 s

RunTime = 40
ExecutionTime = 0.93 s  ClockTime = 1 s

RunTime = 40.125
ExecutionTime = 0.93 s  ClockTime = 1 s

RunTime = 40.25
ExecutionTime = 0.94 s  ClockTime = 1 s

RunTime = 40.375
ExecutionTime = 0.94 s  ClockTime = 1 s

RunTime = 40.5
ExecutionTime = 0.94 s  ClockTime = 1 s

RunTime = 40.625
ExecutionTime = 0.95 s  ClockTime = 1 s

RunTime = 40.75
ExecutionTime = 0.95 s  ClockTime = 1 s

RunTime = 40.875
ExecutionTime = 0.95 s  ClockTime = 1 s

RunTime = 41
ExecutionTime = 0.95 s  ClockTime = 1 s

RunTime = 41.125
ExecutionTime = 0.96 s  ClockTime = 1 s

RunTime = 41.25
ExecutionTime = 0.96 s  ClockTime = 1 s

RunTime = 41.375
ExecutionTime = 0.96 s  ClockTime = 1 s

RunTime = 41.5
ExecutionTime = 0.96 s  ClockTime = 1 s

RunTime = 41.625
ExecutionTime = 0.97 s  ClockTime = 1 s

RunTime = 41.75
ExecutionTime = 0.97 s  ClockTime = 1 s

RunTime = 41.875
ExecutionTime = 0.97 s  ClockTime = 1 s

RunTime = 42
ExecutionTime = 0.98 s  ClockTime = 1 s

RunTime = 42.125
ExecutionTime = 0.98 s  ClockTime = 1 s

RunTime = 42.25
ExecutionTime = 0.98 s  ClockTime = 1 s

RunTime = 42.375
ExecutionTime = 0.98 s  ClockTime = 1 s

RunTime = 42.5
ExecutionTime = 0.99 s  ClockTime = 1 s

RunTime = 42.625
ExecutionTime = 0.99 s  ClockTime = 1 s

RunTime = 42.75
ExecutionTime = 0.99 s  ClockTime = 1 s

RunTime = 42.875
ExecutionTime = 0.99 s  ClockTime = 1 s

RunTime = 43
ExecutionTime = 1 s  ClockTime = 1 s

RunTime = 43.125
ExecutionTime = 1 s  ClockTime = 1 s

RunTime = 43.25
ExecutionTime = 1 s  ClockTime = 1 s

RunTime = 43.375
ExecutionTime = 1.01 s  ClockTime = 1 s

RunTime = 43.5
ExecutionTime = 1.01 s  ClockTime = 1 s

RunTime = 43.625
ExecutionTime = 1.01 s  ClockTime = 1 s

RunTime = 43.75
ExecutionTime = 1.01 s  ClockTime = 1 s

RunTime = 43.875
ExecutionTime = 1.02 s  ClockTime = 1 s

RunTime = 44
ExecutionTime = 1.02 s  ClockTime = 1 s

RunTime = 44.125
ExecutionTime = 1.02 s  ClockTime = 1 s

RunTime = 44.25
ExecutionTime = 1.02 s  ClockTime = 1 s

RunTime = 44.375
ExecutionTime = 1.03 s  ClockTime = 1 s

RunTime = 44.5
ExecutionTime = 1.03 s  ClockTime = 1 s

RunTime = 44.625
ExecutionTime = 1.03 s  ClockTime = 1 s

RunTime = 44.75
ExecutionTime = 1.04 s  ClockTime = 1 s

RunTime = 44.875
ExecutionTime = 1.04 s  ClockTime = 1 s

RunTime = 45
ExecutionTime = 1.04 s  ClockTime = 1 s

RunTime = 45.125
ExecutionTime = 1.04 s  ClockTime = 1 s

RunTime = 45.25
ExecutionTime = 1.05 s  ClockTime = 1 s

RunTime = 45.375
ExecutionTime = 1.05 s  ClockTime = 1 s

RunTime = 45.5
ExecutionTime = 1.05 s  ClockTime = 1 s

RunTime = 45.625
ExecutionTime = 1.05 s  ClockTime = 1 s

RunTime = 45.75
ExecutionTime = 1.06 s  ClockTime = 1 s

RunTime = 45.875
ExecutionTime = 1.06 s  ClockTime = 1 s

RunTime = 46
ExecutionTime = 1.06 s  ClockTime = 1 s

RunTime = 46.125
ExecutionTime = 1.07 s  ClockTime = 1 s

RunTime = 46.25
ExecutionTime = 1.07 s  ClockTime = 1 s

RunTime = 46.375
ExecutionTime = 1.07 s  ClockTime = 1 s

RunTime = 46.5
ExecutionTime = 1.07 s  ClockTime = 1 s

RunTime = 46.625
ExecutionTime = 1.08 s  ClockTime = 1 s

RunTime = 46.75
ExecutionTime = 1.08 s  ClockTime = 1 s

RunTime = 46.875
ExecutionTime = 1.08 s  ClockTime = 1 s

RunTime = 47
ExecutionTime = 1.08 s  ClockTime = 1 s

RunTime = 47.125
ExecutionTime = 1.09 s  ClockTime = 1 s

RunTime = 47.25
ExecutionTime = 1.09 s  ClockTime = 1 s

RunTime = 47.375
ExecutionTime = 1.09 s  ClockTime = 1 s

RunTime = 47.5
ExecutionTime = 1.1 s  ClockTime = 1 s

RunTime = 47.625
ExecutionTime = 1.1 s  ClockTime = 1 s

RunTime = 47.75
ExecutionTime = 1.1 s  ClockTime = 1 s

RunTime = 47.875
ExecutionTime = 1.1 s  ClockTime = 1 s

RunTime = 48
ExecutionTime = 1.11 s  ClockTime = 1 s

RunTime = 48.125
ExecutionTime = 1.11 s  ClockTime = 1 s

RunTime = 48.25
ExecutionTime = 1.11 s  ClockTime = 1 s

RunTime = 48.375
ExecutionTime = 1.11 s  ClockTime = 1 s

RunTime = 48.5
ExecutionTime = 1.12 s  ClockTime = 1 s

RunTime = 48.625
ExecutionTime = 1.12 s  ClockTime = 1 s

RunTime = 48.75
ExecutionTime = 1.12 s  ClockTime = 1 s

RunTime = 48.875
ExecutionTime = 1.13 s  ClockTime = 1 s

RunTime = 49
ExecutionTime = 1.13 s  ClockTime = 1 s

RunTime = 49.125
ExecutionTime = 1.13 s  ClockTime = 1 s

RunTime = 49.25
ExecutionTime = 1.13 s  ClockTime = 1 s

RunTime = 49.375
ExecutionTime = 1.14 s  ClockTime = 1 s

RunTime = 49.5
ExecutionTime = 1.14 s  ClockTime = 1 s

RunTime = 49.625
ExecutionTime = 1.14 s  ClockTime = 1 s

RunTime = 49.75
ExecutionTime = 1.15 s  ClockTime = 1 s

RunTime = 49.875
ExecutionTime = 1.15 s  ClockTime = 1 s

RunTime = 50
ExecutionTime = 1.15 s  ClockTime = 1 s

RunTime = 50.125
ExecutionTime = 1.15 s  ClockTime = 1 s

RunTime = 50.25
ExecutionTime = 1.16 s  ClockTime = 1 s

RunTime = 50.375
ExecutionTime = 1.16 s  ClockTime = 1 s

RunTime = 50.5
ExecutionTime = 1.16 s  ClockTime = 1 s

RunTime = 50.625
ExecutionTime = 1.17 s  ClockTime = 1 s

RunTime = 50.75
ExecutionTime = 1.17 s  ClockTime = 1 s

RunTime = 50.875
ExecutionTime = 1.17 s  ClockTime = 1 s

RunTime = 51
ExecutionTime = 1.17 s  ClockTime = 1 s

RunTime = 51.125
ExecutionTime = 1.18 s  ClockTime = 1 s

RunTime = 51.25
ExecutionTime = 1.18 s  ClockTime = 1 s

RunTime = 51.375
ExecutionTime = 1.18 s  ClockTime = 1 s

RunTime = 51.5
ExecutionTime = 1.18 s  ClockTime = 1 s

RunTime = 51.625
ExecutionTime = 1.19 s  ClockTime = 1 s

RunTime = 51.75
ExecutionTime = 1.19 s  ClockTime = 1 s

RunTime = 51.875
ExecutionTime = 1.19 s  ClockTime = 1 s

RunTime = 52
ExecutionTime = 1.2 s  ClockTime = 1 s

RunTime = 52.125
ExecutionTime = 1.2 s  ClockTime = 1 s

RunTime = 52.25
ExecutionTime = 1.2 s  ClockTime = 1 s

RunTime = 52.375
ExecutionTime = 1.2 s  ClockTime = 1 s

RunTime = 52.5
ExecutionTime = 1.21 s  ClockTime = 1 s

RunTime = 52.625
ExecutionTime = 1.21 s  ClockTime = 1 s

RunTime = 52.75
ExecutionTime = 1.21 s  ClockTime = 1 s

RunTime = 52.875
ExecutionTime = 1.21 s  ClockTime = 1 s

RunTime = 53
ExecutionTime = 1.22 s  ClockTime = 1 s

RunTime = 53.125
ExecutionTime = 1.22 s  ClockTime = 1 s

RunTime = 53.25
ExecutionTime = 1.22 s  ClockTime = 1 s

RunTime = 53.375
ExecutionTime = 1.23 s  ClockTime = 1 s

RunTime = 53.5
ExecutionTime = 1.23 s  ClockTime = 1 s

RunTime = 53.625
ExecutionTime = 1.23 s  ClockTime = 1 s

RunTime = 53.75
ExecutionTime = 1.23 s  ClockTime = 1 s

RunTime = 53.875
ExecutionTime = 1.24 s  ClockTime = 1 s

RunTime = 54
ExecutionTime = 1.24 s  ClockTime = 1 s

RunTime = 54.125
ExecutionTime = 1.24 s  ClockTime = 1 s

RunTime = 54.25
ExecutionTime = 1.24 s  ClockTime = 1 s

RunTime = 54.375
ExecutionTime = 1.25 s  ClockTime = 1 s

RunTime = 54.5
ExecutionTime = 1.25 s  ClockTime = 1 s

RunTime = 54.625
ExecutionTime = 1.25 s  ClockTime = 1 s

RunTime = 54.75
ExecutionTime = 1.26 s  ClockTime = 1 s

RunTime = 54.875
ExecutionTime = 1.26 s  ClockTime = 1 s

RunTime = 55
ExecutionTime = 1.26 s  ClockTime = 1 s

RunTime = 55.125
ExecutionTime = 1.26 s  ClockTime = 1 s

RunTime = 55.25
ExecutionTime = 1.27 s  ClockTime = 1 s

RunTime = 55.375
ExecutionTime = 1.27 s  ClockTime = 1 s

RunTime = 55.5
ExecutionTime = 1.27 s  ClockTime = 1 s

RunTime = 55.625
ExecutionTime = 1.27 s  ClockTime = 1 s

RunTime = 55.75
ExecutionTime = 1.28 s  ClockTime = 1 s

RunTime = 55.875
ExecutionTime = 1.28 s  ClockTime = 1 s

RunTime = 56
ExecutionTime = 1.28 s  ClockTime = 1 s

RunTime = 56.125
ExecutionTime = 1.29 s  ClockTime = 1 s

RunTime = 56.25
ExecutionTime = 1.29 s  ClockTime = 1 s

RunTime = 56.375
ExecutionTime = 1.29 s  ClockTime = 1 s

RunTime = 56.5
ExecutionTime = 1.29 s  ClockTime = 1 s

RunTime = 56.625
ExecutionTime = 1.3 s  ClockTime = 1 s

RunTime = 56.75
ExecutionTime = 1.3 s  ClockTime = 1 s

RunTime = 56.875
ExecutionTime = 1.3 s  ClockTime = 1 s

RunTime = 57
ExecutionTime = 1.3 s  ClockTime = 1 s

RunTime = 57.125
ExecutionTime = 1.31 s  ClockTime = 1 s

RunTime = 57.25
ExecutionTime = 1.31 s  ClockTime = 1 s

RunTime = 57.375
ExecutionTime = 1.31 s  ClockTime = 1 s

RunTime = 57.5
ExecutionTime = 1.32 s  ClockTime = 1 s

RunTime = 57.625
ExecutionTime = 1.32 s  ClockTime = 1 s

RunTime = 57.75
ExecutionTime = 1.32 s  ClockTime = 1 s

RunTime = 57.875
ExecutionTime = 1.32 s  ClockTime = 1 s

RunTime = 58
ExecutionTime = 1.33 s  ClockTime = 1 s

RunTime = 58.125
ExecutionTime = 1.33 s  ClockTime = 1 s

RunTime = 58.25
ExecutionTime = 1.33 s  ClockTime = 1 s

RunTime = 58.375
ExecutionTime = 1.33 s  ClockTime = 1 s

RunTime = 58.5
ExecutionTime = 1.34 s  ClockTime = 1 s

RunTime = 58.625
ExecutionTime = 1.34 s  ClockTime = 1 s

RunTime = 58.75
ExecutionTime = 1.34 s  ClockTime = 1 s

RunTime = 58.875
ExecutionTime = 1.35 s  ClockTime = 1 s

RunTime = 59
ExecutionTime = 1.35 s  ClockTime = 1 s

RunTime = 59.125
ExecutionTime = 1.35 s  ClockTime = 1 s

RunTime = 59.25
ExecutionTime = 1.35 s  ClockTime = 1 s

RunTime = 59.375
ExecutionTime = 1.36 s  ClockTime = 1 s

RunTime = 59.5
ExecutionTime = 1.36 s  ClockTime = 1 s

RunTime = 59.625
ExecutionTime = 1.36 s  ClockTime = 1 s

RunTime = 59.75
ExecutionTime = 1.37 s  ClockTime = 1 s

RunTime = 59.875
ExecutionTime = 1.37 s  ClockTime = 1 s

RunTime = 60
ExecutionTime = 1.37 s  ClockTime = 1 s

RunTime = 60.125
ExecutionTime = 1.37 s  ClockTime = 1 s

RunTime = 60.25
ExecutionTime = 1.38 s  ClockTime = 1 s

RunTime = 60.375
ExecutionTime = 1.38 s  ClockTime = 1 s

RunTime = 60.5
ExecutionTime = 1.38 s  ClockTime = 1 s

RunTime = 60.625
ExecutionTime = 1.39 s  ClockTime = 1 s

RunTime = 60.75
ExecutionTime = 1.39 s  ClockTime = 1 s

RunTime = 60.875
ExecutionTime = 1.39 s  ClockTime = 1 s

RunTime = 61
ExecutionTime = 1.39 s  ClockTime = 1 s

RunTime = 61.125
ExecutionTime = 1.4 s  ClockTime = 1 s

RunTime = 61.25
ExecutionTime = 1.4 s  ClockTime = 1 s

RunTime = 61.375
ExecutionTime = 1.4 s  ClockTime = 1 s

RunTime = 61.5
ExecutionTime = 1.4 s  ClockTime = 1 s

RunTime = 61.625
ExecutionTime = 1.41 s  ClockTime = 1 s

RunTime = 61.75
ExecutionTime = 1.41 s  ClockTime = 1 s

RunTime = 61.875
ExecutionTime = 1.41 s  ClockTime = 1 s

RunTime = 62
ExecutionTime = 1.41 s  ClockTime = 1 s

RunTime = 62.125
ExecutionTime = 1.42 s  ClockTime = 1 s

RunTime = 62.25
ExecutionTime = 1.42 s  ClockTime = 1 s

RunTime = 62.375
ExecutionTime = 1.42 s  ClockTime = 1 s

RunTime = 62.5
ExecutionTime = 1.43 s  ClockTime = 1 s

RunTime = 62.625
ExecutionTime = 1.43 s  ClockTime = 1 s

RunTime = 62.75
ExecutionTime = 1.43 s  ClockTime = 1 s

RunTime = 62.875
ExecutionTime = 1.43 s  ClockTime = 1 s

RunTime = 63
ExecutionTime = 1.44 s  ClockTime = 1 s

RunTime = 63.125
ExecutionTime = 1.44 s  ClockTime = 1 s

RunTime = 63.25
ExecutionTime = 1.44 s  ClockTime = 1 s

RunTime = 63.375
ExecutionTime = 1.44 s  ClockTime = 1 s

RunTime = 63.5
ExecutionTime = 1.45 s  ClockTime = 1 s

RunTime = 63.625
ExecutionTime = 1.45 s  ClockTime = 1 s

RunTime = 63.75
ExecutionTime = 1.45 s  ClockTime = 1 s

RunTime = 63.875
ExecutionTime = 1.46 s  ClockTime = 1 s

RunTime = 64
ExecutionTime = 1.46 s  ClockTime = 1 s

RunTime = 64.125
ExecutionTime = 1.46 s  ClockTime = 1 s

RunTime = 64.25
ExecutionTime = 1.46 s  ClockTime = 1 s

RunTime = 64.375
ExecutionTime = 1.47 s  ClockTime = 1 s

RunTime = 64.5
ExecutionTime = 1.47 s  ClockTime = 1 s

RunTime = 64.625
ExecutionTime = 1.47 s  ClockTime = 1 s

RunTime = 64.75
ExecutionTime = 1.47 s  ClockTime = 1 s

RunTime = 64.875
ExecutionTime = 1.48 s  ClockTime = 1 s

RunTime = 65
ExecutionTime = 1.48 s  ClockTime = 1 s

MassError: 5.17007247044e-07/200.75 = 2.5753785656e-09
TotalEnergy: 0.000319344399278
End

