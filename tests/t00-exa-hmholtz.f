      program t00_exa_hmholtz
      implicit none

      include 'mpif.h'
      include 'exaf.h'

      integer ierr,nargs
      integer exah,hmhz

      character*128 arg

      call mpi_init(ierr)

      nargs=command_argument_count()
      if(nargs.ne.1) then
        write(stderr,*) "Usage: <program> <backend>"
        stop 1
      endif
      call get_command_argument(1,arg)

      call exainit(trim(arg),mpi_comm_world,exah,ierr)
      call exahmholtzcreate(hmhz,exah,ierr)

      call exahmholtzdestroy(hmhz,ierr)
      call exafinalize(exah,ierr)

      call mpi_finalize(ierr)
      end
