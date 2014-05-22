PHP_ARG_ENABLE(pg_dist, whether to enable my extension,
[ --enable-my-extension   Enable my extension])
 
if test "$PHP_PG_DIST" = "yes"; then
  AC_DEFINE(HAVE_PG_DIST, 1, [Whether you have my extension])
  PHP_NEW_EXTENSION(pg_dist, pg_dist.c, $ext_shared)
fi
