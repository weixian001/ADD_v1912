# dualZoneEulerFoam and dualZoneBuoyantPimpleFoam v1912

Authors: Wei Xian Lim, U S Vevek, Wai Lee Chan, Basman Elhadidi

## Description
A domain decomposition technique in the finite volume framework is 
developed based on an overset method that couples a nonlinear solver with a linearized counterpart solver.
The nonlinear domain accounts for complex physics in the near field, 
such as acoustic wave interaction with shock waves and acoustic wave scattering from solid bodies. 
The linear domain allows the small perturbation waves induced by the nonlinear effects to propagate with 
minimal numerical dissipation and dispersion while also significantly reducing computational resources. 
The accuracy of the simulations with DDT is found to depend on the extent of overlap between the 
linear and nonlinear zones. 
In general, an overlap ratio of at least two is necessary to maintain high accuracy.
Furthermore, the implementation of DDT in the external flow case can eliminate numerical noise observed 
at the far field of the fully-nonlinear counterpart, 
which do not have any effective non-reflecting boundary treatment.

## Available Solvers

* dualZoneEulerFoam (Coupling nonlinear and linear Euler equations)

* dualZoneBuoyantPimpleFoam (Coupling linearlized NS and NS with buoyancy effect, adapted from buoyantPimpleFoam)

## Installation

This version works with OpenFOAM-v1912

* Prepare a directory on your system, e.g.:  

  `mkdir ~/OpenFOAM/ADD-v1912/`

* Download FPIFoam-v1912 using git:

  `git clone https://github.com/weixian001/ADD-v1912.git ~/OpenFOAM/ADD-v1912/`

* Set an environment variable to the FPIFoam-v1912 src folder:

  `export LIB_ADD=~/OpenFOAM/ADD-v1912/`

* Execute `./Allwmake`

## Citation

If you use our codes please cite:

```
@article{lim2023domain,
  title={A domain decomposition technique for acoustic wave interactions and scattering},
  author={Lim, Wei Xian and Chan, Wai Lee and Elhadidi, Basman},
  journal={Computers \& Mathematics with Applications},
  volume={152},
  pages={129--142},
  year={2023},
  publisher={Elsevier}
}

@article{vevek2021domain,
  title={A domain decomposition technique for small amplitude wave interactions with shock waves},
  author={Vevek, US and Elhadidi, Basman and Chan, Wai Lee},
  journal={Journal of Computational Physics},
  volume={437},
  pages={110326},
  year={2021},
  publisher={Elsevier}
}
```

## Notes
The solvers are developed by the NanyangCFD team at Nanyang Technological University, Singapore leading by Prof Wai Lee CHAN (chan.wl@ntu.edu.sg). 
Main contributors are U S Vevek and Wei Xian Lim (weixian001@e.ntu.edu.sg).
