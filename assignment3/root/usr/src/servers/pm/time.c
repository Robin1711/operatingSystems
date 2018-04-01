/* This file takes care of those system calls that deal with time.
 *
 * The entry points into this file are
 *   do_time:		perform the TIME system call
 *   do_utctime:	perform the UTCTIME system call 
 *   do_stime:		perform the STIME system call
 *   do_times:		perform the TIMES system call
 */

#include "pm.h"
#include <minix/callnr.h>
#include <minix/com.h>
#include <signal.h>
#include "mproc.h"
#include "param.h"

/*===========================================================================*
 *				do_time					     *
 *===========================================================================*/
int do_time()
{
/* Perform the time(tp) system call. This returns the time in seconds since 
 * 1.1.1970.  MINIX is an astrophysically naive system that assumes the earth 
 * rotates at a constant rate and that such things as leap seconds do not 
 * exist.
 */
  clock_t uptime, boottime;
  int s;

  if ( (s=getuptime2(&uptime, &boottime)) != OK) 
  	panic("do_time couldn't get uptime: %d", s);

  mp->mp_reply.reply_time = (time_t) (boottime + (uptime/system_hz));
  mp->mp_reply.reply_utime = (uptime%system_hz)*1000000/system_hz;
  return(OK);
}

int do_utctime()
{
/* Perform the time(tp) system call. This returns the time in seconds since 
 * 1.1.1970.  MINIX is an astrophysically naive system that assumes the earth 
 * rotates at a constant rate and that such things as leap seconds do not 
 * exist.
 */
  clock_t uptime, boottime;
  int s;

  if ( (s=getuptime2(&uptime, &boottime)) != OK) 
  	panic("do_time couldn't get uptime: %d", s);
  
  int t = (int) (boottime + (uptime/system_hz));
  if (t > 78796799) t++;
  if (t > 94694399) t++;
  if (t > 126230399) t++;
  if (t > 157766399) t++;
  if (t > 189302399) t++;
  if (t > 220924799) t++;
  if (t > 252460799) t++;
  if (t > 283996799) t++;
  if (t > 315532799) t++;
  if (t > 362793599) t++;
  if (t > 394329599) t++;
  if (t > 425865599) t++;
  if (t > 489023999) t++;
  if (t > 567993599) t++;
  if (t > 631151999) t++;
  if (t > 662687999) t++;
  if (t > 709948799) t++;
  if (t > 741484799) t++;
  if (t > 773020799) t++;
  if (t > 820454399) t++;
  if (t > 867715199) t++;
  if (t > 915148799) t++;
  if (t > 1136073599) t++;
  if (t > 1230767999) t++;
  if (t > 1341100799) t++;
  if (t > 1435708799) t++;
  if (t > 1483228799) t++;
  
  mp->mp_reply.reply_time = (time_t) t;

  mp->mp_reply.reply_utime = (uptime%system_hz)*1000000/system_hz;
  return(OK);
}
/*===========================================================================*
 *				do_stime				     *
 *===========================================================================*/
int do_stime()
{
/* Perform the stime(tp) system call. Retrieve the system's uptime (ticks 
 * since boot) and pass the new time in seconds at system boot to the kernel.
 */
  clock_t uptime, boottime;
  int s;

  if (mp->mp_effuid != SUPER_USER) { 
      return(EPERM);
  }
  if ( (s=getuptime(&uptime)) != OK) 
      panic("do_stime couldn't get uptime: %d", s);
  boottime = (long) m_in.stime - (uptime/system_hz);

  s= sys_stime(boottime);		/* Tell kernel about boottime */
  if (s != OK)
	panic("pm: sys_stime failed: %d", s);

  return(OK);
}

/*===========================================================================*
 *				do_times				     *
 *===========================================================================*/
int do_times()
{
/* Perform the times(buffer) system call. */
  register struct mproc *rmp = mp;
  clock_t user_time, sys_time, uptime;
  int s;

  if (OK != (s=sys_times(who_e, &user_time, &sys_time, &uptime, NULL)))
      panic("do_times couldn't get times: %d", s);
  rmp->mp_reply.reply_t1 = user_time;		/* user time */
  rmp->mp_reply.reply_t2 = sys_time;		/* system time */
  rmp->mp_reply.reply_t3 = rmp->mp_child_utime;	/* child user time */
  rmp->mp_reply.reply_t4 = rmp->mp_child_stime;	/* child system time */
  rmp->mp_reply.reply_t5 = uptime;		/* uptime since boot */

  return(OK);
}

