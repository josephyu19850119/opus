/* Copyright (c) 2011-2012 IETF Trust, Xiph.Org Foundation. All rights reserved.
   Written by Gregory Maxwell */
/*

   This file is extracted from RFC6716. Please see that RFC for additional
   information.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   - Neither the name of Internet Society, IETF or IETF Trust, nor the
   names of specific contributors, may be used to endorse or promote
   products derived from this software without specific prior written
   permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
   OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

static inline void deb2_impl(unsigned char *_t,unsigned char **_p,int _k,int _x,int _y)
{
  int i;
  if(_x>2){
     if(_y<3)for(i=0;i<_y;i++)*(--*_p)=_t[i+1];
  }else{
     _t[_x]=_t[_x-_y];
     deb2_impl(_t,_p,_k,_x+1,_y);
     for(i=_t[_x-_y]+1;i<_k;i++){
       _t[_x]=i;
       deb2_impl(_t,_p,_k,_x+1,_x);
     }
  }
}

/*Generates a De Bruijn sequence (k,2) with length k^2*/
static inline void debruijn2(int _k, unsigned char *_res)
{
   unsigned char *p;
   unsigned char *t;
   t=malloc(sizeof(unsigned char)*_k*2);
   memset(t,0,sizeof(unsigned char)*_k*2);
   p=&_res[_k*_k];
   deb2_impl(t,&p,_k,1,1);
   free(t);
}

/*MWC RNG of George Marsaglia*/
static opus_uint32 Rz, Rw;
static inline opus_uint32 fast_rand(void)
{
  Rz=36969*(Rz&65535)+(Rz>>16);
  Rw=18000*(Rw&65535)+(Rw>>16);
  return (Rz<<16)+Rw;
}
static opus_uint32 iseed;

#ifdef __GNUC__
__attribute__((noreturn))
#endif
static inline void _test_failed(const char *file, int line)
{
  fprintf(stderr,"\n ***************************************************\n");
  fprintf(stderr," ***         A fatal error was detected.         ***\n");
  fprintf(stderr," ***************************************************\n");
  fprintf(stderr,"Please report this failure and include\n");
  fprintf(stderr,"'make check SEED=%u fails %s at line %d for %s'\n",iseed,file,line,opus_get_version_string());
  fprintf(stderr,"and any relevant details about your system.\n\n");
  abort();
}
#define test_failed() _test_failed(__FILE__, __LINE__);
