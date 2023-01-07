import { TestBed } from '@angular/core/testing';

import { DeleteBookingService } from './delete-booking.service';

describe('DeleteBookingService', () => {
  let service: DeleteBookingService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(DeleteBookingService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
