eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id: run_test.pl 87490 2009-11-12 08:30:40Z dbudko $
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;

$status = 0;
$debug_level = '0';

foreach $i (@ARGV) {
    if ($i eq '-debug') {
        $debug_level = '10';
    }
}

my $client = PerlACE::TestTarget::create_target (1) || die "Create target 1 failed\n";

$CL = $client->CreateProcess ("client", "-ORBdebuglevel $debug_level");

$client_status = $CL->SpawnWaitKill ($client->ProcessStartWaitInterval() + 50);

if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

exit $status;
