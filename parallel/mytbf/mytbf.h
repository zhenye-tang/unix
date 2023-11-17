#ifndef __MYTBF_H__
#define __MYTBF_H__

#define MYTBF_MAX       1024
typedef void mytbf_t;

mytbf_t *mytbf_init(int cps, int burst);
int mytbf_destory(mytbf_t *);
int mytbf_fetchtoken(mytbf_t *, int );
int mytbf_returntoken(mytbf_t *, int );


#endif //
