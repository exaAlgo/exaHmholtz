      program t00_exa_hmholtz
      implicit none

      include 'mpif.h'
      include 'exaf.h'

      integer ierr,nargs
      integer exah,hmhz,mesh

      real*8 x(8),y(8),z(8)
      integer*8 glonum(8)
      real *8 geom(6,8),mask(8),D(2,2)

      integer test

      character*128 arg

      x=(/-1,1,-1,1,-1,1,-1,1/)
      y=(/-1,-1,1,1,-1,-1,1,1/)
      z=(/-1,-1,-1,-1,1,1,1,1/)

      glonum=(/1,2,3,4,5,6,7,8/)

      geom=0
      mask=0
      D=0

      call mpi_init(ierr)

      nargs=command_argument_count()
      if(nargs.ne.1) then
        write(stderr,*) "Usage: <program> <backend>"
        stop 1
      endif
      call get_command_argument(1,arg)

      call exainit(trim(arg),mpi_comm_world,exah,ierr)

      call exahmholtzcreate(hmhz,exah,ierr)

      call exameshcreate(mesh,exa_str_null,exah,ierr)

      call exameshsetdim(mesh,3,ierr);
      call exameshsetnelements(mesh,1,ierr);
      call exameshset1ddofs(mesh,2,ierr);

      call exameshsetxcoords(mesh,x,ierr);
      call exameshsetycoords(mesh,y,ierr);
      call exameshsetzcoords(mesh,z,ierr);

      call exameshsetglobalnumbering(mesh,glonum,ierr);

      call exameshsetgeometricfactors(mesh,geom,ierr);
      call exameshsetmask(mesh,mask,ierr);
      call exameshsetderivativematrix(mesh,D,ierr);

      call exameshgetdim(test,mesh,ierr)
      if(test.ne.3) then
        write(stderr,*) "exameshgetdim failed."
      endif

      call exameshget1ddofs(test,mesh,ierr)
      if(test.ne.2) then
        write(stderr,*) "exameshget1ddofs failed."
      endif

      call exameshgetnelements(test,mesh,ierr)
      if(test.ne.1) then
        write(stderr,*) "exameshgetnelements failed."
      endif

      call exameshdestroy(mesh,ierr)
      call exahmholtzdestroy(hmhz,ierr)
      call exafinalize(exah,ierr)

      call mpi_finalize(ierr)
      end
